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

// コンストラクタ
AnimationScene::AnimationScene(int StageNum)
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
		{ 0, 0, 27 },		// 視点
		{ 0, 1, 0 },		// 注視点
		{ 0, 1, 0 }			// 上ベクトル
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

	Stage::Instance().SelectStage(StageNum);

	EffectManager::instance().Initialize();
	sceneModel = std::make_unique<SceneModel>("Data/Model/TetrisBlock/scene.mdl");
	sceneScale = { 0.1f, 0.1f, 0.1f };
}

AnimationScene::~AnimationScene() {
	PlayerManager::Instance().Clear();
}

// 更新処理
void AnimationScene::Update(float elapsedTime)
{
	// カメラ更新処理
	cameraController.Update();
	cameraController.SyncControllerToCamera(Camera::Instance());
	//player->Update(elapsedTime);


	//// トランスフォーム更新処理
	//UpdateTransform(elapsedTime);
	if (timer > 1&& Co < 9)
	{
		PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0.0f, 5, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, -90.0f, 0.0f)));
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

	//stageの追加
	//stage.get()->Update(elapsedTime);

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

	TrapManager::Instance().Update(elapsedTime);
	EffectManager::instance().Update(elapsedTime);
	Stage::Instance().Update(elapsedTime);
	KeyManager::Instance().Update(elapsedTime);
	PortalManager::Instance().Update(elapsedTime);
}

// 描画処理
void AnimationScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	Grid2DRenderer* grid2dRenderer = Graphics::Instance().GetGrid2DRenderer();
	Graphics2D* gfx2D = Graphics::Instance().GetGraphics2D();

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
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	EffectManager::instance().Render(Camera::Instance().GetView(), Camera::Instance().GetProjection());
	Stage::Instance().Render(elapsedTime, rc);
	TrapManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	KeyManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);
	PortalManager::Instance().Render(modelRenderer, rc, ShaderId::Lambert);

	sceneModel->SelectedBlockRender(rc, modelRenderer, sceneTransform, 0u, ShaderId::Lambert);
}

//// GUI描画処理
//void AnimationScene::DrawGUI()
//{
//
//	if (ImGui::Begin(u8"アニメーション", nullptr, ImGuiWindowFlags_None))
//	{
//		ImGui::Text(u8"移動操作：WASD");
//		ImGui::Text(u8"ジャンプ操作：Space");
//		ImGui::Spacing();
//
//		const char* stateName = "";
//		ImGui::LabelText("State", stateName);
//		ImGui::InputFloat("velocity",);
//
//		ImGui::End();
//	}
//}

//// アニメーション再生
//void AnimationScene::PlayAnimation(int index, bool loop)
//{
//	animationPlaying = true;
//	animationLoop = loop;
//	animationIndex = index;
//	animationSeconds = 0.0f;
//}
//
//void AnimationScene::PlayAnimation(const char* name, bool loop)
//{
//	int index = 0;
//	const std::vector<ModelResource::Animation>& animations = player->model->GetResource()->GetAnimations();
//	for (const ModelResource::Animation& animation : animations)
//	{
//		if (animation.name == name)
//		{
//			PlayAnimation(index, loop);
//			return;
//		}
//		++index;
//	}
//}


// アニメーション更新処理
//void AnimationScene::UpdateAnimation(float elapsedTime)
//{
//	if (animationPlaying) {
//
//		float blendRate = 1.0f;
//		if (animationSeconds < animationBlendSecondsLength) {
//			blendRate = animationSeconds / animationBlendSecondsLength;
//			blendRate *= blendRate;
//		}
//
//		std::vector<Model::Node>& nodes = player->model->GetNodes();
//
//		//指定のアニメーションデータを取得
//		const std::vector<ModelResource::Animation>& animations = player->model->GetResource()->GetAnimations();
//		const ModelResource::Animation& animation = animations.at(animationIndex);
//
//		//時間経過
//		animationSeconds += elapsedTime;
//
//		//再生時間が終端時間を超えたら
//		if (animationSeconds >= animation.secondsLength) {
//			if (animationLoop) {
//				animationSeconds -= animation.secondsLength;
//			}
//			else {
//				animationPlaying = false;
//				animationSeconds = animation.secondsLength;
//			}
//		}
//		//アニメーションデータからキーフレームデータリストを取得
//		const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
//		int keyCount = static_cast<int>(keyframes.size());
//		for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
//		{
//			const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
//			const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
//
//			if (animationSeconds >= keyframe0.seconds && animationSeconds < keyframe1.seconds) {
//				float rate = (animationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
//				int nodeCount = static_cast<int>(nodes.size());
//				for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex) {
//
//					const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
//					const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);
//
//					Model::Node& node = nodes[nodeIndex];
//
//					if (blendRate < 1.0f) {
//						DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
//						DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
//						DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
//						DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
//						DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
//						DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
//						DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
//						DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
//						DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
//
//						DirectX::XMStoreFloat3(&node.scale, S);
//						DirectX::XMStoreFloat4(&node.rotate, R);
//						DirectX::XMStoreFloat3(&node.translate, T);
//
//					}
//					else {
//						DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
//						DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
//						DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
//						DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
//						DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
//						DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
//						DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
//						DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
//						DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
//
//						DirectX::XMStoreFloat3(&node.scale, S);
//						DirectX::XMStoreFloat4(&node.rotate, R);
//						DirectX::XMStoreFloat3(&node.translate, T);
//
//					}
//				}
//				break;
//			}
//		}
//	}
//	//player->model->UpdateTransform();
//}

	//}
	//	stage->DrawGUI();

	//	ImGui::End();

