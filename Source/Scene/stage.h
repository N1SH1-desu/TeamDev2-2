#pragma once


#include"Camera.h"
#include"FreeCameraController.h"
#include"RenderContext.h"
#include"Model.h"

namespace stage_number
{
	const int stage_max_num = 7;

	
}

class stage
{
private:
	struct Object
	{
		bool					onGround = false;
		DirectX::XMFLOAT3		velocity = { 0, 0, 0 };
		DirectX::XMFLOAT3		position = { 0, 0, 0 };
		DirectX::XMFLOAT3		angle = { 0, 0, 0 };
		DirectX::XMFLOAT3		scale = { 1., 1., 1. };
		DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		std::unique_ptr<Model>	model;

		void UpdateTransform()
		{
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			DirectX::XMMATRIX WorldTransform = S * R * T;
			DirectX::XMStoreFloat4x4(&transform, WorldTransform);
		}
	};
	Object stage_[stage_number::stage_max_num];
	Object stage_collision_[stage_number::stage_max_num];

	int now_stage;
	DirectX::XMFLOAT4X4 stage_transform[stage_number::stage_max_num] =
	{
	{
	1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, 0, 1
	},
	{
	1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, 0, 1
	},
	{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0
	,0,0,0,1},
		{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0
	,0,0,0,1},
		{	
	1,0,0,0,
	0,1,0,0,
	0,0,1,0
	,0,0,0,1},
		{},
		{},
	};
	DirectX::XMFLOAT4X4 stage_collision_transform[stage_number::stage_max_num] =
	{
{
1, 0, 0, 0
, 0, 1, 0, 0
, 0, 0, 1, 0
, 0, 0, 0, 1
},
{
	1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, 0, 1
	},
	{
	1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, 0, 1
	},
	{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0
	,0,0,0,1},
	{	
	1,0,0,0,
	0,1,0,0,
	0,0,1,0
	,0,0,0,1},
	{},
		{},
	};

public:
	Stage();
	~Stage() = default;

	void Update(float elapsedTime);

	// 描画処理
	void Render(float elapsedTime, RenderContext& rc);

	// GUI描画処理
	void DrawGUI();

	void SelectStage(int selector);

	Model* GetModel() { return stage_[now_stage].model.get(); }

	int GetNumber() { return now_stage; }

	DirectX::XMFLOAT4X4 GetTransform() { return stage_[now_stage].transform; }
	DirectX::XMFLOAT4X4 GetCollisionTransform() { return stage_collision_[now_stage].transform; }

	//モデルを覆うように似たような地形の簡易マップを呼び出す
	Model* GetCollisionModel() { return stage_collision_[now_stage].model.get(); }
	

};

