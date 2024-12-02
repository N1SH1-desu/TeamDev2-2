#include <imgui.h>
#include <ImGuizmo.h>
#include "Graphics.h"
#include "Scene/AnimationScene.h"
#include"PlayerManager.h"
#include "Collision.h"
#include "Grid2DRenderer.h"
#include "ControlTetrisBlock.h"

// �R���X�g���N�^
AnimationScene::AnimationScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// �J�����ݒ�
	Camera::Instance().SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		1000.0f								// �t�@�[�N���b�v
	);
	Camera::Instance().SetLookAt(
		{ 0, 0, 27 },		// ���_
		{ 0, 1, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(Camera::Instance());
	PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0, 3, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0, 180, 0)));

	timer = 0;
	cube.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube.position = { -8,1, 0 };
	cube.angle = { 0, 0, 0 };
	cube.scale = { 2, 2, 2 };

	cube2.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube2.position = { 5,1, 0 };
	cube2.angle = { 0, 0, 0 };
	cube2.scale = { 2, 2, 2 };

	stage = std::make_unique<Stage>();

	sceneModel = std::make_unique<SceneModel>("Data/Model/TetrisBlock/scene.mdl");
	sceneScale = { 0.1f, 0.1f, 0.1f };
}

AnimationScene::~AnimationScene() {
	PlayerManager::Instance().Clear();
}

// �X�V����
void AnimationScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(Camera::Instance());
	//player->Update(elapsedTime);


	//// �g�����X�t�H�[���X�V����
	//UpdateTransform(elapsedTime);
	if (timer > 1&& Co<9)
	{
		PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0.0f, 3, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, -90.0f, 0.0f)));
		timer = 0;
		Co++;
	}

	//if(Collision::InteresectCylinderVsCylinder(PlayerManager::Instance().))

	//�����蔻��
	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); i++)
	{
		auto player = PlayerManager::Instance().GetPlayer(i);
		DirectX::XMFLOAT3 outPosition;

		float length = player->position.y - cube.position.y;

	//	if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube.position, 2.0f, 2.0f, outPosition))
	//	{
	//		player->turn();
	//		//player->HitP();

	//		//player->PlayAnimation("Jump", false);
	//		//player->state = Player::State::Jump;
	//	}

	//	else
	//	{
			if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube.position, 2.0f, length, outPosition))
			{
				player->PlayAnimation("Running", true);
				player->state = Player::State::Run;
			}
			else if (player->state == Player::State::Run && !player->onGround)
			{
				player->PlayAnimation("Jump", true);
				player->state = Player::State::Idle;
			}
	//	}

	//	if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube2.position, 2.0f, 2.0f, outPosition))
	//	{
	//		player->turn();
	//	}
	}

	//stage�̒ǉ�
	stage.get()->Update(elapsedTime);

	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	RECT viewport = { 0, 0, static_cast<LONG>(Graphics::Instance().GetScreenWidth()), static_cast<LONG>(Graphics::Instance().GetScreenHeight()) };
	
	scenePosition = SetBlockPosFromMousePos(refInputMouse, Grid2DRenderer::grid_size, viewport, Projection, View, World);

	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sceneScale.x, sceneScale.y, sceneScale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(scenePosition.x, scenePosition.y, scenePosition.z);
		DirectX::XMStoreFloat4x4(&sceneTransform, S * R * T);
	}

	PlayerManager::Instance().Update(elapsedTime);
	cube.UpdateTransform();
	cube2.UpdateTransform();
	timer += elapsedTime;
}

// �`�揈��
void AnimationScene::Render(float elapsedTime)
{


	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	Grid2DRenderer* grid2dRenderer = Graphics::Instance().GetGrid2DRenderer();
	Graphics2D* gfx2D = Graphics::Instance().GetGraphics2D();

	//// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &Camera::Instance();
	//modelRenderer->Render(rc, player->transform, player->model, ShaderId::Lambert);
	//modelRenderer->Render(rc, cube.transform, cube.model.get(), ShaderId::Lambert);
	//modelRenderer->Render(rc, cube2.transform, cube2.model.get(), ShaderId::Lambert);

	PlayerManager::Instance().Render(modelRenderer,rc,ShaderId::Lambert);
	//player->Render(dc);

	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	// �O���b�h�`��
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	stage->Render(elapsedTime,rc);

	sceneModel->SelectedBlockRender(rc, modelRenderer, sceneTransform, 0u, ShaderId::Lambert);

	grid2dRenderer->Draw(gfx2D->GetContext());
}


//// GUI�`�揈��
void AnimationScene::DrawGUI()
{

	//if (ImGui::Begin(u8"�A�j���[�V����", nullptr, ImGuiWindowFlags_None))
	//{
	//	ImGui::Text(u8"�ړ�����FWASD");
	//	ImGui::Text(u8"�W�����v����FSpace");
	//	ImGui::Spacing();

	//}
	//	stage->DrawGUI();

	//	ImGui::End();
}

