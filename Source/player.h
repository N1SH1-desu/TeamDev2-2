#pragma once

#include"Shader.h"
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include"Mathf.h"
#include "FetchModelFromSceneAsset.h"
#include "StageEditer.h"
#include "TetroEditerMode.h"

//player
class Player 
{
public:
	Player(DirectX::XMFLOAT3 position = { 0, 10, 0 }, DirectX::XMFLOAT3 scale = { 0.01, 0.01, 0.01 }, DirectX::XMFLOAT3 angle = { 0, 0, 0 });
	~Player();


	// トランスフォーム更新処理
	void UpdateTransform(float elapsedTime);


	void Update(float elapsedTime, TerrainStage::StageTerrain& terrain, TetroEditerMode& TetroEditer);

	DirectX::XMFLOAT3 GetPosition() { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	float GetRaidus() { return radius; }
	float GetHeight() { return height; }

	void OnLanded() { onGround = true; }


	void PlayAnimation(int index, bool loop);
	void PlayAnimation(const char* name, bool loop);
	void Animation_Reset() { animationSeconds = 0.0f; }

	void UpdateAnimation(float elapsedTime);

	void Clear_Judge();

	void UpdateVerticalMove(TerrainStage::StageTerrain& StageTerrain, TetroEditerMode& TetroEditer, float elapsedTime);
	void UpdateHorizontalMove(TerrainStage::StageTerrain& StageTerrain, TetroEditerMode& TetroEditer, float elapsedTime);

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
		EndJump,
	};
	State								state;
	State								before_state;

	int									animationIndex = -1;
	float								animationSeconds = 0.0f;
	bool								animationLoop = false;
	bool								animationPlaying = false;
	float								animationBlendSecondsLength = 0.2f;

	std::string							animationName;

	Camera								camera;
	FreeCameraController				cameraController;


	DirectX::XMFLOAT3					position = { 0, 5, 0 };
	DirectX::XMFLOAT3					angle = { 0.01, 0, 0 };
	DirectX::XMFLOAT3					scale = { 0.01f, 0.01f, 0.01f };

	DirectX::XMFLOAT3					velocity = { 0, 0, 0 };
	float								gravity = 10.0f;
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
	bool PC = false;
	float PT;

	float								tt;

	float								wal;

	float									Walltime;

	int									HP = 10;

	float radius = 2.0f;
	float height = 2.0f;

	float floatingCount = 0.21f;

	bool HorizonMoveFlag = false;

	float ClimingSpeed = 2.0f;
	float HitWallCount = 0.0f;

	bool HitOverHead = false;
};