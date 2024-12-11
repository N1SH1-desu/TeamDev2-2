#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/ProjectScreenScene.h"

// コンストラクタ
ProjectScreenScene::ProjectScreenScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();
	ID2D1DeviceContext* d2dContext = Graphics::Instance().GetGfx2D()->GetContext();

	// カメラ設定
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// 画角
		screenWidth / screenHeight,			// 画面アスペクト比
		0.1f,								// ニアクリップ
		1000.0f								// ファークリップ
	);
	camera.SetOrthoGraphic(
		128.0,
		72.0,
		0.1f,
		1000.0f
	);
	camera.SetLookAt(
		{ 0, 0, -20 },		// 視点
		{ 0, 0, 0 },		// 注視点
		{ 0, 1, 0 }			// 上ベクトル
	);
	cameraController.SyncCameraToController(camera);

	sprite = std::make_unique<Sprite>(device);

	editerMode.Initialize(device, d2dContext);

	terrain.Initialize(Stage::Stage1);
}

// 更新処理
void ProjectScreenScene::Update(float elapsedTime)
{
	// カメラ更新処理
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	// マウス左クリックした
	if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
	{
		// スクリーンサイズ取得
		float screenWidth = Graphics::Instance().GetScreenWidth();
		float screenHeight = Graphics::Instance().GetScreenHeight();

		// マウスカーソル位置の取得
		POINT cursor;
		::GetCursorPos(&cursor);
		::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

	}

	keyInput.Update();
	POINTS mousePos = refInputMouse->GetPosition();
	editerMode.Update(elapsedTime, mousePos, keyInput);

	stage.position = SetBlockPosFromMousePos(refInputMouse, Grid2DRenderer::grid_size, viewport, Projection, View, World);

	// ステージ行列更新処理
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(stage.scale.x, stage.scale.y, stage.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(stage.angle.x, stage.angle.y, stage.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(stage.position.x, stage.position.y, stage.position.z);
		DirectX::XMStoreFloat4x4(&stage.transform, S * R * T);
	}
}

// 描画処理
void ProjectScreenScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();
	EndlessGridRenderer* gridRenderer = Graphics::Instance().GetEndlessGridRenderer();
	ID2D1DeviceContext* d2dContext = Graphics::Instance().GetGfx2D()->GetContext();

	// モデル描画
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;

	// スクリーンサイズ取得
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	terrain.Render(rc, modelRenderer);

	editerMode.Render(rc, d2dContext, modelRenderer);
}

// GUI描画処理
void ProjectScreenScene::DrawGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();

	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 180), ImGuiCond_Once);

	if (ImGui::Begin(u8"スクリーン座標変換", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::Text(u8"クリック：キャラ配置");

		POINTS pos = refInputMouse->GetPosition();
		int v[2] = { pos.x, pos.y };
		ImGui::InputInt2("Mouse Position", v);

		ImGui::SliderFloat3("Block Position", &stage.position.x, -200.0f, 200.0f);
		ImGui::InputFloat3("Block Pos Input", &stage.position.x);
		ImGui::SliderFloat("Block Scale", &stage.scale.x, 0.001f, 0.1f);
		stage.scale.y = stage.scale.x;
		stage.scale.z = stage.scale.x;
		ImGui::SliderFloat3("Block Rotate", &stage.angle.x, 0.0f, 20.0f);
	}
	ImGui::End();
}