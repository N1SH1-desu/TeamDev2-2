#include "PoisonGus.h"
#include "Collision.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "imgui.h"

void PoisonGus::Initialize()
{

}

void PoisonGus::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();

	Sensing(elapsedTime);
}

void PoisonGus::Sensing(float elapsedTime)
{
	EffectManager& effect = EffectManager::instance();

	DirectX::XMFLOAT3 outPosition;

	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); i++)
	{
		auto player = PlayerManager::Instance().GetPlayer(i);

		if (Collision::InteresectCylinderVsCylinder(position, radius, height, player->GetPosition(), player->GetRaidus(), player->GetHeight(), outPosition))
		{
			if (!effect.GetEffect(POISON)->IsPlay())
			{
				effect.GetEffect(POISON)->Play(position, 0.5f);
				break;
			}
		}
	}
}

void PoisonGus::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	modelRenderer->Render(rc, transform, model.get(), ID);

	EffectManager& effect = EffectManager::instance();

	ImGui::Begin("Gus");
	ImGui::Text("Play : %d", static_cast<int>(effect.GetEffect(POISON)->IsPlay()));
	ImGui::End();
}