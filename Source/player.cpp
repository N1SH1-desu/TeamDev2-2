#include<imgui.h>
#include"Player.h"
#include"Camera.h"
#include"Graphics.h"
#include"Collision.h"
#include"PlayerManager.h"
#include"Scene/stage.h"
#include "PortalManager.h"

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


void Player::Update(float elapsedTime)
{
	tt = elapsedTime;
	InputMove();
	before_state = state;

	switch (state)
	{
	case State::Idle:
		position.y -= velocity.y;
		wal = 0.03f;
		if (RayGround(Stage::Instance().GetCollisionTransform(), Stage::Instance().GetCollisionModel()))
		{
			state = Run;
		}
		break;

	case State::Run:
		if (RayGround(Stage::Instance().GetCollisionTransform(), Stage::Instance().GetCollisionModel())) {
		position.x -= moveSpeed * elapsedTime;
		onGround;
		PlayAnimation("Run", true);
		}
		else {
		position.y -= velocity.y;
			PlayAnimation("Falling", true);

		}
		break;

	case State::Jump:
		position.y += wal;
		PlayAnimation("Climing", true);

		break;
	case State::EndJump:
		position.x -= moveSpeed * elapsedTime;
		break;
	}

	if (PC)
	{
		PoisonC(elapsedTime);
	}

	// トランスフォーム更新処理
	UpdateTransform(elapsedTime);

	for (int i = 0; i < PortalManager::Instance().GetObjectCount(); i++)
	{
		auto portal = PortalManager::Instance().GetObject_(i);

		DirectX::XMFLOAT3 outPosition;
		//if (Collision::InteresectCylinderVsCylinder(position, radius, height, portal->GetPosition(), portal->GetRadius(), portal->GetHeight(), outPosition) && portal->Enabled())
		//{
			//PlayerManager::Instance().Remove(this);
		//}
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
	velocity.y = gravity * elapsedTime;

		// 行列計算
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMStoreFloat4x4(&transform, S * R * T);
}



// 移動入力処理
bool Player::InputMove()
{
		// 入力処理
		float axisX = 0.0f;
		float axisY = 0.0f;
		/*if (GetAsyncKeyState('W') & 0x8000) axisY += 1.0f;
		if (GetAsyncKeyState('S') & 0x8000) axisY -= 1.0f;*/

		if (onGround==true)axisX -= moveSpeed;

		/*if (GetAsyncKeyState('A') & 0x8000) axisX -= 1.0f;*/

		// カメラの方向
		const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
		const DirectX::XMFLOAT3& camemraRight = camera.GetRight();
		float cameraFrontLengthXZ = sqrtf(cameraFront.x * cameraFront.x + cameraFront.z * cameraFront.z);
		float cameraRightLengthXZ = sqrtf(camemraRight.x * camemraRight.x + camemraRight.z * camemraRight.z);
		float cameraFrontX = cameraFront.x / cameraFrontLengthXZ;
		float cameraFrontZ = cameraFront.z / cameraFrontLengthXZ;
		float cameraRightX = camemraRight.x / cameraRightLengthXZ;
		float cameraRightZ = camemraRight.z / cameraRightLengthXZ;

		// 移動ベクトル
		moveVecX = cameraFrontX * axisY + cameraRightX * axisX*tt;
		moveVecZ = cameraFrontZ * axisY + cameraRightZ * axisX*tt;
		float vecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

		// 壁ずり移動処理
		if (vecLength > 0)
		{
			//レイの始点と終点を求める
			DirectX::XMFLOAT3 s = {
			   position.x,
			   position.y + 0.5f,
			   position.z
			};

			DirectX::XMFLOAT3 e = {
			   position.x + moveVecX,
			   position.y + 0.5f,
			   position.z + moveVecZ
			};

			DirectX::XMFLOAT3 p, n;
			//ステージとレイキャストを行い、交点と法線を求める
			if (Collision::RayCast(s, e, Stage::Instance().GetCollisionTransform(), Stage::Instance().GetCollisionModel(), p, n)) {
				//交点から終点へのベクトルを求める
				DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
				DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&e);
				DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

				//三角関数から終点へのベクトルを求める
				DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
				DirectX::XMVECTOR A = DirectX::XMVector3Dot(N, PE);
				//壁までの長さを少しだけ長くなるように補正する
				float a = -DirectX::XMVectorGetX(A) + 0.001f;

				//壁ずりベクトルを求める
				DirectX::XMVECTOR R = DirectX::XMVectorAdd(PE, DirectX::XMVectorScale(N, a));

				//壁ずり後の位置を求める
				DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);
				DirectX::XMFLOAT3 q;
				DirectX::XMStoreFloat3(&q, Q);

				Walltime += tt;

				if (Walltime < 1.0f) {
					state = State::Jump;
				}
				else
				{
					turn();
					state = State::Run;
					Walltime = 0.0f;
				}
			}
		}
	return true;

}


void Player::turn() {
		angle.y *= -1.0f;
		moveSpeed *= -1.0f;
}

void Player::Death()
{
	PlayerManager::Instance().Remove(this);
}

void Player::HitP()
{
	HP--;
	if (HP < 0) {
		Death();
	}
}

void Player::PoisonC(float elapsedTime)
{
	PT += elapsedTime;
	if (PT / 0.5f >= 1.0f)
	{
		HP--;
		if (HP < 0) {
			Death();
		}
		PT = 0;
	}
}

bool Player::RayGround(DirectX::XMFLOAT4X4 transform, Model* model)
{

	const DirectX::XMFLOAT3 s = { position.x,position.y + 0.5f,position.z };
	const DirectX::XMFLOAT3 e = { position.x,position.y - 0.3f,position.z };

	DirectX::XMFLOAT3 p, n;

	if (Collision::RayCast(s, e, transform, model, p, n))
	{
		// 交点のY座標をプレイヤーに位置に設定する
		velocity.y = 0;
		position.y = p.y;
		return onGround = true;
	}
	return onGround=false;
}

