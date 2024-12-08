#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/ProjectScreenScene.h"
#include "ControlTetrisBlock.h"

// コンストラクタ
ProjectScreenScene::ProjectScreenScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

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
	stage.model = std::make_unique<Model>("Data/Model/Stage/ExampleStage.mdl");

	sceneModels = std::make_unique<SceneModel>("Data/Model/TetrisBlock/Colors.mdl");

	stage.scale = { 8.0f, 8.0f, 8.0f };
	stage.position = { 0.0f, 0.0f, 0.0f };
	stage.angle = { 0.0f, 0.0f, 0.0f };
	
	editerUI.Initialize(device);
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

	{
		if (keyInput.GetKeyStatus('G') == Input::Release)
		{
			EditerMode = !EditerMode;
		}

		{
			Grid2DRenderer* g2R = Graphics::Instance().GetGridRenderer();
			g2R->Update(elapsedTime, EditerMode);
		}

		if (EditerMode)
		{
			POINTS mousePos = refInputMouse->GetPosition();

			tetroEditer.Update(mousePos, keyInput, sceneModels.get());
		}
	}

	{
		static bool hoge = false;
		if (keyInput.GetKeyStatus(VK_TAB) == Input::Release)
		{
			hoge = !hoge;
		}
		editerUI.Update(elapsedTime, hoge);
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
	Grid2DRenderer* grid2DRenderer = Graphics::Instance().GetGridRenderer();

	// モデル描画
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	//modelRenderer->Render(rc, stage.transform, stage.model.get(), ShaderId::Lambert);

	// スクリーンサイズ取得
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	for (const Object& obj : objs)
	{
		//modelRenderer->Render(rc, obj.transform, obj.model.get(), ShaderId::Lambert);
	}

	if (EditerMode)
	{
		tetroEditer.Render(rc, modelRenderer);
	}
	grid2DRenderer->Draw(d2dContext);

	editerUI.Render(dc);
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
		ImGui::SliderFloat3("Block Scale", &stage.scale.x, 0.01f, 10.0f);
		ImGui::SliderFloat3("Block Rotate", &stage.angle.x, 0.0f, 20.0f);
	}
	ImGui::End();
}
