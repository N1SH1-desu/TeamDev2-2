#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/ProjectScreenScene.h"
#include "ControlTetrisBlock.h"

// �R���X�g���N�^
ProjectScreenScene::ProjectScreenScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// �J�����ݒ�
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		1000.0f								// �t�@�[�N���b�v
	);
	camera.SetLookAt(
		{ 0, 0, -40 },		// ���_
		{ 0, 0, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	sprite = std::make_unique<Sprite>(device);
	stage.model = std::make_unique<Model>("Data/Model/Stage/ExampleStage.mdl");

	sceneModels = std::make_unique<SceneModel>("Data/Model/TetrisBlock/fixedScene.mdl");

	stage.scale = { 0.125f, 0.125f, 0.125f };
}

// �X�V����
void ProjectScreenScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	// �}�E�X���N���b�N����
	if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
	{
		// �X�N���[���T�C�Y�擾
		float screenWidth = Graphics::Instance().GetScreenWidth();
		float screenHeight = Graphics::Instance().GetScreenHeight();

		// �}�E�X�J�[�\���ʒu�̎擾
		POINT cursor;
		::GetCursorPos(&cursor);
		::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

	}

	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());	
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	RECT viewport = { 0, 0, static_cast<LONG>(Graphics::Instance().GetScreenWidth()), static_cast<LONG>(Graphics::Instance().GetScreenHeight()) };

	//stage.position = SetBlockPosFromMousePos(refInputMouse, Grid2DRenderer::grid_size, viewport, Projection, View, World);

	// �X�e�[�W�s��X�V����
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(stage.scale.x, stage.scale.y, stage.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(stage.angle.x, stage.angle.y, stage.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(stage.position.x, stage.position.y, stage.position.z);
		DirectX::XMStoreFloat4x4(&stage.transform, S * R * T);
	}

	// �I�u�W�F�N�g�s��X�V����
	for (Object& obj : objs)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(obj.scale.x, obj.scale.y, obj.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(obj.angle.x, obj.angle.y, obj.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(obj.position.x, obj.position.y, obj.position.z);
		DirectX::XMStoreFloat4x4(&obj.transform, S * R * T);
	}
}

// �`�揈��
void ProjectScreenScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	Graphics2D* d2dGraphics = Graphics::Instance().GetGraphics2D();
	Grid2DRenderer* gridRenderer = Graphics::Instance().GetGrid2DRenderer();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();

	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	//modelRenderer->Render(rc, stage.transform, stage.model.get(), ShaderId::Lambert);

	// �X�N���[���T�C�Y�擾
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	for (const Object& obj : objs)
	{
		modelRenderer->Render(rc, obj.transform, obj.model.get(), ShaderId::Lambert);
	}

	//sceneModels->SelectedBlockRender(rc, modelRenderer, stage.transform, 0u, ShaderId::Lambert);

	//gridRenderer->Draw(d2dGraphics->GetContext());

	// �O���b�h�`��
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

// GUI�`�揈��
void ProjectScreenScene::DrawGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();

	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 180), ImGuiCond_Once);

	if (ImGui::Begin(u8"�X�N���[�����W�ϊ�", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::Text(u8"�N���b�N�F�L�����z�u");

		POINTS pos = refInputMouse->GetPosition();
		int v[2] = { pos.x, pos.y };
		ImGui::InputInt2("Mouse Position", v);

		ImGui::InputFloat3("Block Position", &stage.position.x);
		ImGui::InputFloat3("Block Scale", &stage.scale.x);
		ImGui::InputFloat3("Block Rotate", &stage.angle.x);
	}
	ImGui::End();
}
