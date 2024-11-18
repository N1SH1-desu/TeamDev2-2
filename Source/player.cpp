#include<imgui.h>
#include"Player.h"
#include"Camera.h"
#include"Graphics.h"
#include"Collision.h"
#include"PlayerManager.h"

 Player::Player(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle) {


	model = new Model("./Data/Model/Jammo/Jammo.mdl");
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->state = State::Idle;
	this->HP = 10;
	PlayAnimation("Jump", false);
}

 Player::~Player() {
	delete model;
}


void Player::Update(float elapsedTime)
{
	InputMove();
	//// �X�e�[�g����
	//switch (state)
	//{
	//case State::Idle:
	//{
	//	if (onGround==true && InputMove()) {
	//		state = State::Run;
	//		PlayAnimation("Running", true);
	//	}
	//	break;
	//}

	//case State::Run:
	//{
	//	if (!InputMove()) {
	//		state = State::Idle;
	//		PlayAnimation("Idle", true);
	//	}
	//	if (InputJump()) {
	//		state = State::Jump;
	//		PlayAnimation("Jump", false);
	//	}

	//	break;
	//}

	//case State::Jump:
	//{
	//	bool move = InputMove();

	//	if (onGround) {
	//		if (move) {
	//			state = State::Run;
	//			PlayAnimation("Running", true);
	//		}
	//		else {
	//			state = State::Idle;
	//			PlayAnimation("Idle", true);
	//		}
	//	}
	//	break;
	//}
	//}

	switch (state)
	{
	case State::Idle:
		position.y -= velocity.y;

		if (onGround)
		{
			//Animation_Reset();
			PlayAnimation("Running", true);
			state = Run;
		}
		break;

	case State::Run:
		position.x -= moveSpeed * elapsedTime;
		break;

	case State::Jump:
		position.y += velocity.y;
		break;
	}

	if (position.y <= 0.0f)
		onGround = true;
	else
		onGround = false;

	// �g�����X�t�H�[���X�V����
	UpdateTransform(elapsedTime);

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

		//�w��̃A�j���[�V�����f�[�^���擾
		const std::vector<ModelResource::Animation>& animations = model->GetResource()->GetAnimations();
		const ModelResource::Animation& animation = animations.at(animationIndex);

		//���Ԍo��
		animationSeconds += elapsedTime;

		//�Đ����Ԃ��I�[���Ԃ𒴂�����
		if (animationSeconds >= animation.secondsLength) {
			if (animationLoop) {
				animationSeconds -= animation.secondsLength;
			}
			else {
				animationPlaying = false;
				animationSeconds = animation.secondsLength;
			}
		}
		//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
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



// �g�����X�t�H�[���X�V����
void Player::UpdateTransform(float elapsedTime)
{
	//// ��������
	//	float vecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
	//	if (vecLength > 0)
	//	{
	//		float vecX = moveVecX / vecLength;
	//		float vecZ = moveVecZ / vecLength;

	//		float acceleration = this->acceleration * elapsedTime;
	//		if (!onGround) acceleration *= airControl;

	//		velocity.x += vecX * acceleration;
	//		velocity.z += vecZ * acceleration;

	//		// �ő呬�x����
	//		float velocityLength = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	//		if (velocityLength > moveSpeed)
	//		{
	//			velocity.x = (velocity.x / velocityLength) * moveSpeed;
	//			velocity.z = (velocity.z / velocityLength) * moveSpeed;
	//		}

	//		// �i�s�����������悤�ɂ���
	//		{
	//			// �����Ă������
	//			float frontX = sinf(angle.y);
	//			float frontZ = cosf(angle.y);

	//			// ��]�ʒ���
	//			float dot = frontX * vecX + frontZ * vecZ;
	//			float rot = (std::min)(1.0f - dot, turnSpeed * elapsedTime);

	//			// ���E��������ĉ�]����
	//			float cross = frontX * vecZ - frontZ * vecX;
	//			if (cross < 0.0f)
	//			{
	//				angle.y += rot;
	//			}
	//			else
	//			{
	//				angle.y -= rot;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		// ��������
	//		float deceleration = this->deceleration * elapsedTime;
	//		if (!onGround) deceleration *= airControl;

	//		float velocityLength = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	//		if (velocityLength > deceleration)
	//		{
	//			velocity.x -= (velocity.x / velocityLength) * deceleration;
	//			velocity.z -= (velocity.z / velocityLength) * deceleration;
	//		}
	//		else
	//		{
	//			velocity.x = 0.0f;
	//			velocity.z = 0.0f;
	//		}
	//	}

	//	// �ʒu�X�V
	//	position.x += velocity.x * elapsedTime;
	//	position.z +=velocity.z * elapsedTime;
	velocity.y = gravity * elapsedTime;

		// �s��v�Z
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMStoreFloat4x4(&transform, S * R * T);
}



// �ړ����͏���
bool Player::InputMove()
{
		// ���͏���
		float axisX = 0.0f;
		float axisY = 0.0f;
		/*if (GetAsyncKeyState('W') & 0x8000) axisY += 1.0f;
		if (GetAsyncKeyState('S') & 0x8000) axisY -= 1.0f;*/

		if (onGround==true)axisX -= moveSpeed;

		/*if (GetAsyncKeyState('A') & 0x8000) axisX -= 1.0f;*/

		// �J�����̕���
		const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
		const DirectX::XMFLOAT3& camemraRight = camera.GetRight();
		float cameraFrontLengthXZ = sqrtf(cameraFront.x * cameraFront.x + cameraFront.z * cameraFront.z);
		float cameraRightLengthXZ = sqrtf(camemraRight.x * camemraRight.x + camemraRight.z * camemraRight.z);
		float cameraFrontX = cameraFront.x / cameraFrontLengthXZ;
		float cameraFrontZ = cameraFront.z / cameraFrontLengthXZ;
		float cameraRightX = camemraRight.x / cameraRightLengthXZ;
		float cameraRightZ = camemraRight.z / cameraRightLengthXZ;

		// �ړ��x�N�g��
		moveVecX = cameraFrontX * axisY + cameraRightX * axisX;
		moveVecZ = cameraFrontZ * axisY + cameraRightZ * axisX;
		float vecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

	return true;
}

// �W�����v���͏���
bool Player::InputJump()
{
	if (jumpC==true)
	{
			velocity.y = jumpSpeed;
			velocity.x += 1.0f;
			//InputMove();
		return true;
	}
	return false;
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

