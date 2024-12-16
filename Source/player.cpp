#include<imgui.h>
#include"Player.h"
#include"Camera.h"
#include"Graphics.h"
#include"Collision.h"
#include"PlayerManager.h"
#include"Scene/stage.h"
#include "PortalManager.h"
#include "FetchModelFromSceneAsset.h"

#include"space_division_raycast.h"
#include "PortalManager.h"
#include "clear.h"

static constexpr float SCREEN_MIN = -62.0f;
static constexpr float SCREEN_MAX = 62.0f;


 Player::Player(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle) 
 {
	model = new Model("./Data/Model/UnityChan/UnityChan.mdl");
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->state = State::Idle;
	this->HP = 30;
	PlayAnimation("Falling", true);
}

 Player::~Player() {
	delete model;
}


void Player::Update(float elapsedTime, TerrainStage::StageTerrain& terrain, TetroEditerMode& TetroEditer)
{
	tt = elapsedTime;

	if (onGround || hitWall) floatingCount = 0.0f; else floatingCount += elapsedTime;

	if (floatingCount > 0.2f) //一定時間以上浮いている場合
		HorizonMoveFlag = false;
	else HorizonMoveFlag = true;


	UpdateVerticalMove(terrain, TetroEditer, elapsedTime);
	UpdateHorizontalMove(terrain, TetroEditer, elapsedTime);

	// トランスフォーム更新処理
	UpdateTransform(elapsedTime);

	for (int i = 0; i < PortalManager::Instance().GetObjectCount(); i++)
	{
		auto portal = PortalManager::Instance().GetObject_(i);

		if (Collision::InteresectCylinderVsCylinder(position, radius, height, portal->GetPosition(), portal->GetRadius(), portal->GetHeight(), DirectX::XMFLOAT3(0, 0, 0)))
		{
			PlayerManager::Instance().Remove(this);
		}
	}
}
void Player::PlayAnimation(int index, bool loop)
{
	animationPlaying = true;
	animationLoop = loop;
	animationIndex = index;
}

void Player::PlayAnimation(const char* name, bool loop)
{
	int index = 0;
	const std::vector<ModelResource::Animation>& animations = model->GetResource()->GetAnimations();
	if (animationName != static_cast<std::string>(name))
		animationSeconds = 0.0f;

	animationName = name;

	for (const ModelResource::Animation& animation : animations)
	{
		if (animation.name == name)
		{
			PlayAnimation(index, loop);
			return;
		}
		++index;
	}
}
void Player::UpdateAnimation(float elapsedTime)
{
	if (animationPlaying) {

		float blendRate = 1.0f;
		if (animationSeconds < animationBlendSecondsLength) {
			blendRate = animationSeconds / animationBlendSecondsLength;
			blendRate *= blendRate;
		}

		std::vector<Model::Node>& nodes = model->GetNodes();

		//指定のアニメーションデータを取得
		const std::vector<ModelResource::Animation>& animations = model->GetResource()->GetAnimations();
		const ModelResource::Animation& animation = animations.at(animationIndex);

		//時間経過
		animationSeconds += elapsedTime;

		//再生時間が終端時間を超えたら
		if (animationSeconds >= animation.secondsLength) {
			if (animationLoop) {
				animationSeconds -= animation.secondsLength;
			}
			else {
				animationPlaying = false;
				animationSeconds = animation.secondsLength;
			}
		}
		//アニメーションデータからキーフレームデータリストを取得
		const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
		int keyCount = static_cast<int>(keyframes.size());
		for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
		{
			const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
			const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

			if (animationSeconds >= keyframe0.seconds && animationSeconds < keyframe1.seconds) {
				float rate = (animationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
				int nodeCount = static_cast<int>(nodes.size());
				for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex) {

					const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
					const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

					Model::Node& node = nodes[nodeIndex];

					if (blendRate < 1.0f) {
						DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
						DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
						DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
						DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
						DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
						DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
						DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
						DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
						DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

						DirectX::XMStoreFloat3(&node.scale, S);
						DirectX::XMStoreFloat4(&node.rotate, R);
						DirectX::XMStoreFloat3(&node.translate, T);

					}
					else {
						DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
						DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
						DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
						DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
						DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
						DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
						DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
						DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
						DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

						DirectX::XMStoreFloat3(&node.scale, S);
						DirectX::XMStoreFloat4(&node.rotate, R);
						DirectX::XMStoreFloat3(&node.translate, T);

					}
				}
				break;
			}
		}
	}
	model->UpdateTransform();
}

// トランスフォーム更新処理
void Player::UpdateTransform(float elapsedTime)
{
	// 行列計算
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);
}

void Player::UpdateVerticalMove(TerrainStage::StageTerrain& StageTerrain, TetroEditerMode& TetroEditer, float elapsedTime)
{
	velocity.y -= gravity * elapsedTime;
	float moveY = velocity.y * elapsedTime;
	onGround = false;
	HitOverHead = false;

	if (velocity.y < 0.0)
	{
		DirectX::XMFLOAT3 start, end;
		start = end = position;
		
		start.y += 0.5f;
		end.y += moveY;

		auto TerrainModel = StageTerrain.GetTerrainModels();

		DirectX::XMFLOAT3 hitPosition, hitNormal;

		for (auto& Transform : StageTerrain.GetTerrainAndWorlds())
		{
			if (Collision::RayCast(start, end, Transform.second, TerrainModel->GetSceneModels().at(Transform.first).get(), hitPosition, hitNormal))
			{
				position.y = hitPosition.y;
				velocity.y = 0.0f;
				moveY = 0.0f;
				onGround = true;
			}
		}

		for (auto& tetroElement : TetroEditer.GetSceneModel()->GetCommited())
		{
			if (Collision::RayCast(start, end, tetroElement.second, TetroEditer.GetSceneModel()->GetSceneModels().at(tetroElement.first).get(), hitPosition, hitNormal))
			{
				position.y = hitPosition.y;
				velocity.y = 0.0f;
				moveY = 0.0f;
				onGround = true;
			}
		}
	}

	if (!HorizonMoveFlag)
	{
		position.y += moveY;
		PlayAnimation("Falling", true);
	}

	if (hitWall)
	{
		DirectX::XMFLOAT3 start, end;
		start = end = position;

		end.y += 5.0f;

		auto TerrainModel = StageTerrain.GetTerrainModels();

		DirectX::XMFLOAT3 hitPosition, hitNormal;

		for (auto& Transform : StageTerrain.GetTerrainAndWorlds())
		{
			if (Collision::RayCast(start, end, Transform.second, TerrainModel->GetSceneModels().at(Transform.first).get(), hitPosition, hitNormal))
				HitOverHead = true;
		}

		for (auto& tetroElement : TetroEditer.GetSceneModel()->GetCommited())
		{
			if (Collision::RayCast(start, end, tetroElement.second, TetroEditer.GetSceneModel()->GetSceneModels().at(tetroElement.first).get(), hitPosition, hitNormal))
				HitOverHead = true;
		}

		if (!HitOverHead)
		{
			position.y += ClimingSpeed * elapsedTime;
			PlayAnimation("Climing", true);
		}
		else
		{
			angle.y *= -1;
		}
	}
}


void Player::UpdateHorizontalMove(TerrainStage::StageTerrain& StageTerrain, TetroEditerMode& TetroEditer, float elapsedTime)
{
	// プレイヤーの位置と向いている方向を基にレイの始点と方向を設定
    DirectX::XMFLOAT3 start = DirectX::XMFLOAT3(position.x, position.y + 0.5f, position.z);
    DirectX::XMFLOAT3 rayDirection(sinf(angle.y), 0.0f, cosf(angle.y));

    // レイの終点を計算
    float mx = rayDirection.x * 1.5f;
    float mz = rayDirection.z * 1.5f;
    DirectX::XMFLOAT3 end = DirectX::XMFLOAT3(position.x + mx, position.y + 0.5f, position.z + mz);

    DirectX::XMFLOAT3 hitPosition, hitNormal;
    hitWall = false;

    // テトロエディターのオブジェクトに対してレイキャストを行う
    for (auto& tetroElement : TetroEditer.GetSceneModel()->GetCommited())
    {
        if (Collision::RayCast(start, end, tetroElement.second, TetroEditer.GetSceneModel()->GetSceneModels().at(tetroElement.first).get(), hitPosition, hitNormal))  
			hitWall = true;
    }

	//ステージに対してレイキャスト
	auto TerrainModel = StageTerrain.GetTerrainModels();
	for (auto& Transform : StageTerrain.GetTerrainAndWorlds())
	{
		if (Collision::RayCast(start, end, Transform.second, TerrainModel->GetSceneModels().at(Transform.first).get(), hitPosition, hitNormal))
			hitWall = true;
	}

    // 壁にヒットしていない場合、プレイヤーを移動させる
    if (HorizonMoveFlag && !hitWall)
    {
		PlayAnimation("Run", true);
        position.x += (mx * moveSpeed) * elapsedTime;
        position.z += (mz * moveSpeed) * elapsedTime;
    }

	if(position.x > SCREEN_MAX || position.x < SCREEN_MIN) angle.y *= -1;
}

void Player::Clear_Judge()
{
	
}