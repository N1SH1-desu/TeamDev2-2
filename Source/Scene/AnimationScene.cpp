#include <imgui.h>
#include <ImGuizmo.h>
#include "Graphics.h"
#include "Scene/AnimationScene.h"
#include"PlayerManager.h"
#include "Collision.h"

// �R���X�g���N�^
AnimationScene::AnimationScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
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
		{ 0, 5, 27 },		// ���_
		{ 0, 1, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);
	PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0, 3, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0, 180, 0)));

	timer = 0;
	cube.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube.position = { -5,1, 0 };
	cube.angle = { 0, 0, 0 };
	cube.scale = { 2, 2, 2 };

	cube2.model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	cube2.position = { 5,1, 0 };
	cube2.angle = { 0, 0, 0 };
	cube2.scale = { 2, 2, 2 };
}

AnimationScene::~AnimationScene() {
	PlayerManager::Instance().Clear();
}

// �X�V����
void AnimationScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);
	//player->Update(elapsedTime);


	//// �g�����X�t�H�[���X�V����
	//UpdateTransform(elapsedTime);
	if (timer > 1&& Co<9)
	{
		PlayerManager::Instance().Register(new Player(DirectX::XMFLOAT3(0.0f, 3, 0), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, -90.0f, 0.0f)));
		timer = 0;
		Co++;
	}

	//if(Collision::InteresectCylinderVsCylinder(PlayerManager::Instance().))

	//�����蔻��
	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); i++)
	{
		auto player = PlayerManager::Instance().GetPlayer(i);
		DirectX::XMFLOAT3 outPosition;

		float length = player->position.y - cube.position.y;

		if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube.position, 2.0f, 2.0f, outPosition))
		{
			//player->turn();

			player->PlayAnimation("Jump", false);
			player->state = Player::State::Jump;
		}

		else
		{
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
		}

		if (Collision::InteresectCylinderVsCylinder(player->GetPosition(), 2.0f, 2.0f, cube2.position, 2.0f, 2.0f, outPosition))
		{
			player->turn();
		}
	}

	PlayerManager::Instance().Update(elapsedTime);
	cube.UpdateTransform();
	cube2.UpdateTransform();
	//player->Update(elapsedTime);
	// �A�j���[�V�����X�V����
	//UpdateAnimation(elapsedTime);
	timer += elapsedTime;
}

// �`�揈��
void AnimationScene::Render(float elapsedTime)
{


	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();

	//// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	//modelRenderer->Render(rc, player->transform, player->model, ShaderId::Lambert);
	modelRenderer->Render(rc, cube.transform, cube.model.get(), ShaderId::Lambert);
	modelRenderer->Render(rc, cube2.transform, cube2.model.get(), ShaderId::Lambert);

	PlayerManager::Instance().Render(modelRenderer,rc,ShaderId::Lambert);
	//player->Render(dc);

	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	// �O���b�h�`��
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}


//// GUI�`�揈��
//void AnimationScene::DrawGUI()
//{
//
//	if (ImGui::Begin(u8"�A�j���[�V����", nullptr, ImGuiWindowFlags_None))
//	{
//		ImGui::Text(u8"�ړ�����FWASD");
//		ImGui::Text(u8"�W�����v����FSpace");
//		ImGui::Spacing();
//
//		const char* stateName = "";
//		ImGui::LabelText("State", stateName);
//		ImGui::InputFloat("velocity",);
//
//		ImGui::End();
//	}
//}

//// �A�j���[�V�����Đ�
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


// �A�j���[�V�����X�V����
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
//		//�w��̃A�j���[�V�����f�[�^���擾
//		const std::vector<ModelResource::Animation>& animations = player->model->GetResource()->GetAnimations();
//		const ModelResource::Animation& animation = animations.at(animationIndex);
//
//		//���Ԍo��
//		animationSeconds += elapsedTime;
//
//		//�Đ����Ԃ��I�[���Ԃ𒴂�����
//		if (animationSeconds >= animation.secondsLength) {
//			if (animationLoop) {
//				animationSeconds -= animation.secondsLength;
//			}
//			else {
//				animationPlaying = false;
//				animationSeconds = animation.secondsLength;
//			}
//		}
//		//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
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

// �g�����X�t�H�[���X�V����
//void AnimationScene::UpdateTransform(float elapsedTime)
//{
//	// ��������
//	float vecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
//	if (vecLength > 0)
//	{
//		float vecX = moveVecX / vecLength;
//		float vecZ = moveVecZ / vecLength;
//
//		float acceleration = this->acceleration * elapsedTime;
//		if (!onGround) acceleration *= airControl;
//
//		velocity.x += vecX * acceleration;
//		velocity.z += vecZ * acceleration;
//
//		// �ő呬�x����
//		float velocityLength = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
//		if (velocityLength > moveSpeed)
//		{
//			velocity.x = (velocity.x / velocityLength) * moveSpeed;
//			velocity.z = (velocity.z / velocityLength) * moveSpeed;
//		}
//
//		// �i�s�����������悤�ɂ���
//		{
//			// �����Ă������
//			float frontX = sinf(angle.y);
//			float frontZ = cosf(angle.y);
//
//			// ��]�ʒ���
//			float dot = frontX * vecX + frontZ * vecZ;
//			float rot = (std::min)(1.0f - dot, turnSpeed * elapsedTime);
//
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
//
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
//
//	// �d�͏���
//	velocity.y -= gravity * elapsedTime;
//
//	// �ʒu�X�V
//	position.x += velocity.x * elapsedTime;
//	position.y += velocity.y * elapsedTime;
//	position.z += velocity.z * elapsedTime;
//
//	// �n�ʔ���
//	if (position.y < 0.0f)
//	{
//		position.y = 0.0f;
//		velocity.y = 0.0f;
//		onGround = true;
//	}
//	else
//	{
//		onGround = false;
//	}
//
//
//	// �s��v�Z
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	DirectX::XMStoreFloat4x4(&transform, S * R * T);
//}

// �ړ����͏���
//bool AnimationScene::InputMove()
//{
//	// ���͏���
//	float axisX = 0.0f;
//	float axisY = 0.0f;
//	if (GetAsyncKeyState('W') & 0x8000) axisY += 1.0f;
//	if (GetAsyncKeyState('S') & 0x8000) axisY -= 1.0f;
//	if (onGround) axisX += 1.0f;
//	if (GetAsyncKeyState('A') & 0x8000) axisX -= 1.0f;
//
//	// �J�����̕���
//	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
//	const DirectX::XMFLOAT3& camemraRight = camera.GetRight();
//	float cameraFrontLengthXZ = sqrtf(cameraFront.x * cameraFront.x + cameraFront.z * cameraFront.z);
//	float cameraRightLengthXZ = sqrtf(camemraRight.x * camemraRight.x + camemraRight.z * camemraRight.z);
//	float cameraFrontX = cameraFront.x / cameraFrontLengthXZ;
//	float cameraFrontZ = cameraFront.z / cameraFrontLengthXZ;
//	float cameraRightX = camemraRight.x / cameraRightLengthXZ;
//	float cameraRightZ = camemraRight.z / cameraRightLengthXZ;
//
//	// �ړ��x�N�g��
//	moveVecX = cameraFrontX * axisY + cameraRightX * axisX;
//	moveVecZ = cameraFrontZ * axisY + cameraRightZ * axisX;
//	float vecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
//
//	return vecLength > 0.0f;
//}

// �W�����v���͏���
//bool AnimationScene::InputJump()
//{
//	if (GetAsyncKeyState(VK_SPACE) & 0x01)
//	{
//		velocity.y = jumpSpeed;
//		return true;
//	}
//	return false;
//}
