#include "Key.h"
#include "imgui.h"
#include "PlayerManager.h"
#include "KeyManager.h"
#include "Collision.h"

void Key::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();

	Behavior(elapsedTime); //‹““®
}

void Key::Behavior(float elapsedTime)
{
	Interval += elapsedTime;

	if (Interval > 1.0f)
	{
		magnitication *= -1;
		Interval = 0.0f;
	}

	position.y += (VerticalSpeed * magnitication) * elapsedTime;
	angle.y += TurnSpeed * elapsedTime;

	PlayerManager& players = PlayerManager::Instance();

	for (int i = 0; i < players.GetPlayerCount(); i++)
	{
		auto player = players.GetPlayer(i);

		DirectX::XMFLOAT3 outPosition;

		if (Collision::InteresectCylinderVsCylinder(position, radius, height, player->GetPosition(), player->GetRaidus(), player->GetHeight(), outPosition))
		{
			KeyManager::Instance().Remove(this);
		}
	}
}

void Key::DrawGUI()
{
	ImGui::Begin("easing");
	ImGui::InputFloat3("angle", &angle.x);
	ImGui::End();
}