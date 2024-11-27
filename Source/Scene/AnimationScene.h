#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include"player.h"
#include "stage.h"

class AnimationScene : public Scene
{
public:
	AnimationScene();
	~AnimationScene();

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(float elapsedTime) override;


	// GUI�`�揈��
	//void DrawGUI() override;

private:
	// �A�j���[�V�����Đ�

	// �A�j���[�V�����X�V����
	//void UpdateAnimation(float elapsedTime);

	// �g�����X�t�H�[���X�V����
	//void UpdateTransform(float elapsedTime);

	// �ړ����͏���
	//bool InputMove();

	// �W�����v���͏���
	//bool InputJump();

private:
	std::unique_ptr<Player> player = nullptr;

	Camera								camera;
	FreeCameraController				cameraController;

	struct Object
	{
		DirectX::XMFLOAT3		position = { 0, 0, 0 };
		DirectX::XMFLOAT3		angle = { 0, 0, 0 };
		DirectX::XMFLOAT3		scale = { 1, 1, 1 };
		DirectX::XMFLOAT4X4		transform = { 
			1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			0, 0, 0, 1 };
		std::unique_ptr<Model>	model;

		void UpdateTransform()
		{
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			DirectX::XMMATRIX WorldTransform = S * R * T;
			DirectX::XMStoreFloat4x4(&transform, WorldTransform);
			model->UpdateTransform();
		}
	};
	Object								cube;
	Object								cube2;

	//std::unique_ptr<Stage> stage = nullptr;

	//int									animationIndex = -1;
	//float								animationSeconds = 0.0f;
	//bool								animationLoop = false;
	//bool								animationPlaying = false;
	//float								animationBlendSecondsLength = 0.2f;

	//enum class State
	//{
	//	Idle,
	//	Run,
	//	Jump,
	//};
	//State								state = State::Run;

	float									timer = 0;
	int										Co = 0;
};
