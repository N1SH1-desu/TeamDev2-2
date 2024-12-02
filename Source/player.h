#pragma once

#include"Shader.h"
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include"Mathf.h"


//player
class Player 
{
public:
	Player(DirectX::XMFLOAT3 position = { 0, 10, 0 }, DirectX::XMFLOAT3 scale = { 0.01, 0.01, 0.01 }, DirectX::XMFLOAT3 angle = { 0, 0, 0 });
	~Player();



	// トランスフォーム更新処理
	void UpdateTransform(float elapsedTime);

	// 移動入力処理
	bool InputMove();

	// ジャンプ入力処理
	bool InputJump();

	void Update(float elapsedTime);

	DirectX::XMFLOAT3 GetPosition() { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }


	void OnLanded() { onGround = true; }


	void PlayAnimation(int index, bool loop);
	void PlayAnimation(const char* name, bool loop);
	void Animation_Reset() { animationSeconds = 0.0f; }

	void UpdateAnimation(float elapsedTime);

	void turn();

	void Death();

	void HitP();

	void PoisonC(float elapsedTime);

	bool RayGround(DirectX::XMFLOAT4X4 transform, Model* model);

public:
	Model* model = nullptr;
	bool								onGround = false;
	DirectX::XMFLOAT4X4					transform = { 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

public:

	enum State
	{
		Idle,
		Run,
		Jump,
	};
	State								state;

	int									animationIndex = -1;
	float								animationSeconds = 0.0f;
	bool								animationLoop = false;
	bool								animationPlaying = false;
	float								animationBlendSecondsLength = 0.2f;

	std::string							animationName;

	Camera								camera;
	FreeCameraController				cameraController;


	DirectX::XMFLOAT3					position = { 0, 5, 0 };
	DirectX::XMFLOAT3					angle = { 0, 0, 0 };
	DirectX::XMFLOAT3					scale = { 0.01f, 0.01f, 0.01f };

	DirectX::XMFLOAT3					velocity = { 0, 0, 0 };
	float								gravity = 5.0f;
	float								acceleration = 50.0f;
	float								deceleration = 20.0f;
	float								turnSpeed = DirectX::XMConvertToRadians(720);
	float								jumpSpeed = 7.0f;
	float								airControl = 0.3f;
	float								moveVecX = 0.0f;
	float								moveVecZ = 0.0f;

	bool								hitWall = false;

	float								moveSpeed = 5.0f;

	bool								jumpC = false;

	int									HP = 0;
	bool PC = true;
	float PT;

	float								tt;
};