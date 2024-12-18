#include "TrapManager.h"
#include "EffectManager.h"
#include "Rock.h"
#include "TrapManager.h"


void Rock::Initialize()
{
	
}

void Rock::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();
	
	UpdateVerticalMove(elapsedTime);
}

void Rock::UpdateVerticalMove(float elapsedTime)
{
	velocity.y = gravity * elapsedTime;

	position.y += velocity.y;

	DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };  //足元より上からレイを出す
	DirectX::XMFLOAT3 end = { position.x, position.y + velocity.y - 0.05f, position.z };  //移動後の位置

	/*HitResult hit;*/
	/*if (StageManager::Instance().RayCast(start, end, hit))
	{
		TrapManager::Instance().Remove(this);
		EffectManager::instance().GetEffect(SMOKE)->Play(position, 0.7f);
	}*/
}