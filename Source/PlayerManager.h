#pragma once
#include"Shader.h"
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include"ModelRenderer.h"
#include"Sprite.h"

#include"player.h"
#include<vector>
#include<set>

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager(){}
	static PlayerManager& Instance() {
		static PlayerManager instance;
		return instance;
	}
	void Update(float elapsedTime);

	void Register(Player* player);

	int GetPlayerCount()const { return static_cast<int>(players.size()); }

	Player* GetPlayer(int index) { return players.at(index); }

	void Clear();

	void AllDelete();

	void Remove(Player* player);

	void Render(ModelRenderer* modelRenderer,RenderContext& rc, ShaderId ID);



private:
	std::vector<Player*> players;
	std::set<Player*> remove;

	std::unique_ptr<Sprite>				sprite;

};

