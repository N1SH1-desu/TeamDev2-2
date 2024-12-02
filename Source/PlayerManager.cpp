#include"PlayerManager.h"
#include"Collision.h"
#include "imgui.h"
#include"Graphics.h"

PlayerManager::PlayerManager()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	sprite = std::make_unique<Sprite>(device);
}

void PlayerManager::Update(float elapsedTime) {

	for (Player* player : players) {
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

	//当たり判定
}
void PlayerManager::Render(ModelRenderer* modelRenderer,RenderContext& rc, ShaderId ID) 
{	
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	// スクリーンサイズ取得
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	for (Player* player : players) {
		//頭上のワールド座標
		DirectX::XMFLOAT3 pos = { player->transform._41, player->transform._42, player->transform._43 };
		pos.y += 2.0f;


		//ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&pos);
		DirectX::XMVECTOR ScreenPosition;
		ScreenPosition = DirectX::XMVector3Project(Position, 0.0f, 0.0f,
			screenWidth, screenHeight,
			0.0f, 1.0f,
			Projection, View, World);
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);


		//ゲージ描画
		const float gaugeWidth = player->HP;
		const float gaugeHeight = 5.0f;

		sprite->Render(dc,
			screenPosition.x - 15.0f, screenPosition.y, 0,
			30.0f, gaugeHeight,
			0,
			1, 1, 1, 1
		);
		sprite->Render(dc,
			screenPosition.x - 15.0f, screenPosition.y, 0,
			gaugeWidth, gaugeHeight,
			0,
			1, 0, 0, 1
		);

		modelRenderer->Render(rc, player->transform, player->model, ID);
	}

	ImGui::Begin("Data");

	/*ImGui::Text("onGround : %d", players[0]->onGround);
	ImGui::Text("State : %d", players[0]->state);*/
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