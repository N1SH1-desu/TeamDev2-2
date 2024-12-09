#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/ProjectScreenScene.h"

// �R���X�g���N�^
ProjectScreenScene::ProjectScreenScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();
	ID2D1DeviceContext* d2dContext = Graphics::Instance().GetGfx2D()->GetContext();

	// �J�����ݒ�
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		1000.0f								// �t�@�[�N���b�v
	);
	camera.SetOrthoGraphic(
		128.0,
		72.0,
		0.1f,
		1000.0f
	);
	camera.SetLookAt(
		{ 0, 0, -20 },		// ���_
		{ 0, 0, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	sprite = std::make_unique<Sprite>(device);
	stage.model = std::make_unique<Model>("Data/Model/Stage/ExampleStage.mdl");

	//sceneModels = std::make_unique<SceneModel>("Data/Model/TetrisBlock/Colors.mdl");
	//
	//editerUI.Initialize(device);

	editerMode.Initialize(device, d2dContext);
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

	keyInput.Update();
	POINTS mousePos = refInputMouse->GetPosition();
	editerMode.Update(elapsedTime, mousePos, keyInput);

	//{
	//	if (keyInput.GetKeyStatus('G') == Input::Release)
	//	{
	//		EditerMode = !EditerMode;
	//	}

	//	{
	//		Grid2DRenderer* g2R = Graphics::Instance().GetGridRenderer();
	//		g2R->Update(elapsedTime, EditerMode);
	//	}

	//	if (EditerMode)
	//	{
	//		POINTS mousePos = refInputMouse->GetPosition();

	//		tetroEditer.Update(mousePos, keyInput, sceneModels.get());
	//	}
	//}

	//{
	//	static bool hoge = false;
	//	if (keyInput.GetKeyStatus(VK_TAB) == Input::Release)
	//	{
	//		hoge = !hoge;
	//	}
	//	editerUI.Update(elapsedTime, hoge);
	//}
}

// �`�揈��
void ProjectScreenScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();
	EndlessGridRenderer* gridRenderer = Graphics::Instance().GetEndlessGridRenderer();
	ID2D1DeviceContext* d2dContext = Graphics::Instance().GetGfx2D()->GetContext();

	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;

	// �X�N���[���T�C�Y�擾
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//if (EditerMode)
	//{
	//	tetroEditer.Render(rc, modelRenderer);
	//}
	//grid2DRenderer->Draw(d2dContext);

	//editerUI.Render(dc);
	editerMode.Render(rc, d2dContext, modelRenderer);
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

		ImGui::SliderFloat3("Block Position", &stage.position.x, -200.0f, 200.0f);
		ImGui::InputFloat3("Block Pos Input", &stage.position.x);
		ImGui::SliderFloat3("Block Scale", &stage.scale.x, 0.01f, 10.0f);
		ImGui::SliderFloat3("Block Rotate", &stage.angle.x, 0.0f, 20.0f);
	}
	ImGui::End();
}
