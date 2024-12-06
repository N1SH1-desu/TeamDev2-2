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
			{
				POINTS mPoints = refInputMouse->GetPosition();

				static int rotate = 0;
				static int x = 0;
				static int y = 0;

				int xGrid = mPoints.x / static_cast<SHORT>(Grid2DRenderer::grid_size);
				int yGrid = mPoints.y / static_cast<SHORT>(Grid2DRenderer::grid_size);

				if (tetroCollision.DetectionCollide(static_cast<Tetromino::TetrominoType>(tetroType), yGrid, xGrid, rotate))
				{
					x = xGrid;
					y = yGrid;
				}

				tetroRenderer.CalcWorldPosition(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate);
				tetroRenderer.UpdateTransform(stage.scale);

				if (keyInput.GetKeyStatus('F') == Input::Release)
				{
					rotate++;
					if (rotate > 3)
					{
						rotate = 0;
					}
				}

				if (keyInput.GetKeyStatus(VK_SPACE) == Input::Release)
				{
					if (tetroCollision.PlaceTetromino(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate))
					{
						auto transforms = tetroRenderer.GetTransforms();
						for (DirectX::XMFLOAT4X4 ts : transforms)
						{
							sceneModels->CommitBlock({ 0u, std::move(ts) });
						}
					}
				}
			}
		}
	}



	// ステージ行列更新処理
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(stage.scale.x, stage.scale.y, stage.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(stage.angle.x, stage.angle.y, stage.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(stage.position.x, stage.position.y, stage.position.z);
		DirectX::XMStoreFloat4x4(&stage.transform, S * R * T);
	}

	// オブジェクト行列更新処理
	for (Object& obj : objs)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(obj.scale.x, obj.scale.y, obj.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(obj.angle.x, obj.angle.y, obj.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(obj.position.x, obj.position.y, obj.position.z);
		DirectX::XMStoreFloat4x4(&obj.transform, S * R * T);
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
		{
			for (const DirectX::XMFLOAT4X4& tf : tetroRenderer.GetTransforms())
			{
				sceneModels->SelectedBlockRender(rc, modelRenderer, tf, 0u, ShaderId::Lambert, true);
			}
		}
	}

	sceneModels->RenderCommitedBlocks(rc, modelRenderer, ShaderId::Lambert, true);
	grid2DRenderer->Draw(d2dContext);
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
