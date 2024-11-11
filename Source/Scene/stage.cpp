#include"stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"

Stage::Stage()
{
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
		{ 0, 10, -20 },		// ���_
		{ 0, 10, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	stage_1 = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	
}

void Stage::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	stage_1.get()->UpdateTransform();
}

void Stage::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	modelRenderer->Render(rc,
		{ 0.005, 0, 0, 0
		, 0, 0.005, 0, 0
		, 0, 0, 0.005, 0
		, 0, 10, 0, 1 }
	, stage_1.get(), ShaderId::Lambert);

#if _DEBUG
	// �O���b�h�`��
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
#endif
}

void Stage::DrawGUI()
{
	Graphics* graphics = &Graphics::Instance();

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(ImVec2(300, 720));
	float window_alpha = 0.25f;
	ImGui::SetNextWindowBgAlpha(window_alpha);

	if (ImGui::Begin(u8"GameScene", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		DirectX::XMFLOAT3 camera_pos = camera.GetEye();
		DirectX::XMFLOAT3 camera_focus = camera.GetFocus();
		ImGui::SliderFloat3("camera", &camera_pos.x, -10.f, 10.f, "%.3f");

		camera.SetLookAt(camera_pos, camera_focus, camera.GetUp());
		cameraController.SyncCameraToController(camera);

	}
	ImGui::End();
}
