#include"PlayerManager.h"
#include"Collision.h"
#include "StageManager.h"
#include "imgui.h"

void PlayerManager::Update(float elapsedTime) {

	for (Player* player : players) 
	{
		player->Update(elapsedTime);
		player->UpdateAnimation(elapsedTime);
	}

	for(Player* player : remove) {
		std::vector<Player*>::iterator it = 
			std::find(players.begin(), players.end(), player);
		if(it!=players.end()) {
			players.erase(it);
		}
		delete player;
	}
	remove.clear();

	//“–‚½‚è”»’è
	for (auto player : players)
	{
		HitResult hit;
		DirectX::XMFLOAT3 start = { player->position.x, player->position.y + 1.0f, player->position.z };
		DirectX::XMFLOAT3 end = { player->position.x, player->position.y + player->velocity.y - 0.05f, player->position.z };

		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//Remove(player);
		}
	}
}
void PlayerManager::Render(ModelRenderer* modelRenderer,RenderContext& rc, ShaderId ID) 
{	
	for (Player* player : players) {
		modelRenderer->Render(rc, player->transform, player->model, ID);
	}

	ImGui::Begin("Data");

	ImGui::Text("onGround : %d", players[0]->onGround);
	ImGui::Text("State : %d", players[0]->state);
	ImGui::End();
}


void PlayerManager::Register(Player* player) {
	players.emplace_back(player);
}


void PlayerManager::Clear() {
	for (Player* player : players) {
		delete player;
	}
	//players.resize(0);
	players.clear();
}

void PlayerManager::AllDelete()
{
	for (Player* player : players) {
		delete player;
	}
	players.resize(0);
}

void PlayerManager::Remove(Player* player) {
	remove.insert(player);
}