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

#include "clear.h"

//add_by_nikaido
#include"pause.h"
#include"space_division_raycast.h"

bool pause = false;

AnimationScene::AnimationScene(int StageNum) : StageNumber(StageNum)
{
	EffectManager::instance().Initialize();
}

AnimationScene::~AnimationScene()
{
	PlayerManager::Instance().Clear();
	SpaceDivisionRayCast::Instance().Clear();
}

void AnimationScene::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// カメラ設定
	Camera::Instance().SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// 画角
		screenWidth / screenHeight,			// 画面アスペクト比
		0.1f,								// ニアクリップ
		1000.0f								// ファークリップ
	);
	Camera::Instance().SetLookAt(
		{ 0, 0, -20 },		// 視点
		{ 0, 0, 0 },		// 注視点
		{ 0, 1, 0 }			// 上ベクトル
	);

	Camera::Instance().SetOrthoGraphic(128.0f, 72.0f, 0.1f, 1000.0f);

	cameraController.SyncCameraToController(Camera::Instance());
	
	timer = 0;
	cube.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube.position = { -8,1, 0 };
	cube.angle = { 0, 0, 0 };
	cube.scale = { 2, 2, 2 };

	cube2.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube2.position = { 5,1, 0 };
	cube2.angle = { 0, 0, 0 };
	cube2.scale = { 2, 2, 2 };

	sceneModel = std::make_unique<SceneModel>("Data/Model/TetrisBlock/scene.mdl");
	sceneScale = { 0.1f, 0.1f, 0.1f };

	pause = false;
	Pause::Instance().SetPause(pause);
	ID2D1DeviceContext* dc_2D = Graphics::Instance().GetGraphics2D()->GetContext();

	EditerMode.Initialize(device, dc_2D);
	terrain.Initialize(static_cast<TerrainStage::StageNumber>(StageNumber));


	//add_by_nikaido_iichiko
	SpaceDivisionRayCast::Instance().Load(Stage::Instance().GetCollisionModel());

	pause = false;
	Pause::Instance().SetPause(pause);
	Pause::Instance().SetStageNum(StageNumber);
	ObjectSetting(StageNumber);
	PlayerManager::Instance().Initialize();
	//audio
	//game_bgm_ = Audio::Instance().LoadAudioSource("./Data/Audio/game.wav");

	Clear::Instance().Initialize();
}

// 更新処理
void AnimationScene::Update(float elapsedTime)
{
	// カメラ更新処理
	cameraController.Update();
	cameraController.SyncControllerToCamera(Camera::Instance());
	//player->Update(elapsedTime);

	//game_bgm_->Play(true);

	//// トランスフォーム更新処理
	//UpdateTransform(elapsedTime);
	if (timer > 1&& Co < 5)
	{
		PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(-20.0f, 44, 0), DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f), DirectX::XMFLOAT3(0.0f, 90.0f, 0.0f)));
		timer = 0;
		Co++;
	}

	//if(Collision::InteresectCylinderVsCylinder(PlayerManager::Instance().))

	//当たり判定
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
			//if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube.position, 2.0f, length, outPosition))
			//{
			//	player->PlayAnimation("Running", true);
			//	player->state = Player::State::Run;
			//}
			//else if (player->state == Player::State::Run && !player->onGround)
			//{
			//	player->PlayAnimation("Jump", true);
			//	player->state = Player::State::Idle;
			//}
	//	}

	//	if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube2.position, 2.0f, 2.0f, outPosition))
	//	{
	//		player->turn();
	//	}
	}

	//stageの追加
	//stage.get()->Update(elapsedTime);

	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	RECT viewport = { 0, 0, static_cast<LONG>(Graphics::Instance().GetScreenWidth()), static_cast<LONG>(Graphics::Instance().GetScreenHeight()) };
	
	scenePosition = SetBlockPosFromMousePos(Grid2DRenderer::grid_size, viewport, Projection, View, World);

	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sceneScale.x, sceneScale.y, sceneScale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(scenePosition.x, scenePosition.y, scenePosition.z);
		DirectX::XMStoreFloat4x4(&sceneTransform, S * R * T);
	}

	PlayerManager::Instance().Update(elapsedTime, terrain);
	cube.UpdateTransform();
	cube2.UpdateTransform();
	timer += elapsedTime;

	TrapManager::Instance().Update(elapsedTime);
	EffectManager::instance().Update(elapsedTime);
	KeyManager::Instance().Update(elapsedTime);
	PortalManager::Instance().Update(elapsedTime);
	Pause::Instance().Update(elapsedTime);

	POINTS mousePos = InputMouse::Instance().GetPosition();
	keyinput.Update();
	EditerMode.Update(elapsedTime, mousePos, keyinput, terrain.GetStagePlaced());

	Clear::Instance().Update(elapsedTime);

	if (keyinput.GetKeyStatus('P') == Input::Release)
	{
		pause = !pause;
		Pause::Instance().SetPause(pause);
	}
}

// 描画処理
void AnimationScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	//Grid2DRenderer* grid2dRenderer = Graphics::Instance().GetGrid2DRenderer();
	Graphics2D* gfx2D = Graphics::Instance().GetGraphics2D();
	ID2D1DeviceContext* dc_2D = Graphics::Instance().GetGraphics2D()->GetContext();

	//// モデル描画
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &Camera::Instance();
	//modelRenderer->Render(rc, player->transform, player->model, ShaderId::Lambert);
	//modelRenderer->Render(rc, cube.transform, cube.model.get(), ShaderId::Lambert);
	//modelRenderer->Render(rc, cube2.transform, cube2.model.get(), ShaderId::Lambert);

	PlayerManager::Instance().Render(modelRenderer,rc,ShaderId::Lambert);
	//player->Render(dc);

	// レンダーステート設定
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	// グリッド描画
	//primitiveRenderer->DrawGrid(20, 1);
	//primitiveRenderer->Render(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	EffectManager::instance().Render(Camera::Instance().GetView(), Camera::Instance().GetProjection());
	TrapManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	KeyManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	PortalManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	Pause::Instance().Render(elapsedTime);

	sceneModel->SelectedBlockRender(rc, modelRenderer, sceneTransform, 0u, ShaderId::Lambert);
	EditerMode.Render(rc, dc_2D, modelRenderer);

	terrain.Render(rc, modelRenderer);

	Clear::Instance().Render(elapsedTime);
}

void AnimationScene::ObjectSetting(int StageNum)
{
	switch (StageNum)
	{
	case 0:
		PortalManager::Instance().Register(new Portal("Data/Model/portal/portal.mdl", DirectX::XMFLOAT3(60.0f, -20.0f, 0.0f), DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f)));
		KeyManager::Instance().Register(new Key("Data/Model/key/key.mdl", DirectX::XMFLOAT3(60.0f, 27.0f, 0.0f), DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f)));
		KeyManager::Instance().Register(new Key("Data/Model/key/key.mdl", DirectX::XMFLOAT3(10.0f, 7.5f, 0.0f), DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f)));
		break;
	}
}