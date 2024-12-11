#include <imgui.h>
#include <ImGuizmo.h>
#include "Graphics.h"
#include "Scene/AnimationScene.h"
#include "PlayerManager.h"
#include "Collision.h"
#include "Grid2DRenderer.h"
#include "ControlTetrisBlock.h"
#include "TrapManager.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyManager.h"
#include "PortalManager.h"
#include"Scene/ProjectScreenScene.h"
#include"space_division_raycast.h"


// �R���X�g���N�^
AnimationScene::AnimationScene(int StageNum)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// �J�����ݒ�
	Camera::Instance().SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		10000.0f								// �t�@�[�N���b�v
	);
	Camera::Instance().SetLookAt(
		{ 0, 0, 27 },		// ���_
		{ 0, 1, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(Camera::Instance());
	//PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0, 3, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0, 180, 0)));

	timer = 0;

	Stage::Instance().SelectStage(StageNum);

	//SpaceDivisionRayCast::Instance().Load(Stage::Instance().GetModel());

	EffectManager::instance().Initialize();
	//sceneModel = std::make_unique<SceneModel>("Data/Model/TetrisBlock/scene.mdl");
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
	if (timer > 1&& Co < 9)
	{
		PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0.0f, 5, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, -90.0f, 0.0f)));
		timer = 0;
		Co++;
	}
	//if(Collision::InteresectCylinderVsCylinder(PlayerManager::Instance().))


	//stage�̒ǉ�
	//stage.get()->Update(elapsedTime);

	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	RECT viewport = { 0, 0, static_cast<LONG>(Graphics::Instance().GetScreenWidth()), static_cast<LONG>(Graphics::Instance().GetScreenHeight()) };
	
	//scenePosition = SetBlockPosFromMousePos(refInputMouse, Grid2DRenderer::grid_size, viewport, Projection, View, World);

	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sceneScale.x, sceneScale.y, sceneScale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(scenePosition.x, scenePosition.y, scenePosition.z);
		DirectX::XMStoreFloat4x4(&sceneTransform, S * R * T);
	}

	PlayerManager::Instance().Update(elapsedTime);
	timer += elapsedTime;

	TrapManager::Instance().Update(elapsedTime);
	EffectManager::instance().Update(elapsedTime);
	Stage::Instance().Update(elapsedTime);
	KeyManager::Instance().Update(elapsedTime);
	PortalManager::Instance().Update(elapsedTime);
	ProjectScreenScene::Instance().SetInputMouse(this->refInputMouse);
	ProjectScreenScene::Instance().Update(elapsedTime);

}

// �`�揈��
void AnimationScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	//Grid2DRenderer* grid2dRenderer = Graphics::Instance().GetGrid2DRenderer();
	//Graphics2D* gfx2D = Graphics::Instance().GetGraphics2D();

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


	EffectManager::instance().Render(Camera::Instance().GetView(), Camera::Instance().GetProjection());
	Stage::Instance().Render(elapsedTime, rc);
	TrapManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	KeyManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	PortalManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	ProjectScreenScene::Instance().Render(elapsedTime);

	//sceneModel->SelectedBlockRender(rc, modelRenderer, sceneTransform, 0u, ShaderId::Lambert);
}


