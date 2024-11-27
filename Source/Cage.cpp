#include "Cage.h"
#include "PlayerManager.h"
#include "Collision.h"
#include "StageManager.h"

void Cage::Initialize()
{
	Floor = std::make_unique<Object>("Data/Model/Floor.mdl", DirectX::XMFLOAT3(5.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
}

void Cage::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();
	Floor->Update(elapsedTime);

	Sensing(elapsedTime);

	if (fall)
		UpdateVerticalMove(elapsedTime);
}

void Cage::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	modelRenderer->Render(rc, transform, model.get(), ID);
	Floor->Render(modelRenderer, rc, ID);
}

void Cage::Sensing(float elapsedTime)
{
	DirectX::XMFLOAT3 outPosition;

	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); i++)
	{
		auto player = PlayerManager::Instance().GetPlayer(i);

		if (Collision::InteresectCylinderVsCylinder(Floor->GetPosition(), Floor->GetRadius(), Floor->GetHeight(), player->GetPosition(), player->GetRaidus(), player->GetHeight(), outPosition))
			fall = true;
	}
}

void Cage::UpdateVerticalMove(float elapsedTime)
{
	DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };
	DirectX::XMFLOAT3 end = { position.x, position.y + velocity.y - 0.05f, position.z };

	HitResult hit;
	if (!StageManager::Instance().RayCast(start, end, hit))
	{
		velocity.y = gravity * elapsedTime;
		position.y += velocity.y;
	}
}