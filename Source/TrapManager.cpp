#include "TrapManager.h"
#include "Rock.h"
#include "PoisonGus.h"
#include "Cage.h"

void TrapManager::Initialize()
{
	Interval = 0.0f;

	//Register(new Rock("./Data/Model/Rock.mdl", DirectX::XMFLOAT3(1.0f, 10.0f, 1.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	//Register(new PoisonGus("./Data/Model/Floor.mdl", DirectX::XMFLOAT3(-5.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	//Register(new Cage("./Data/Model/Cage.mdl", DirectX::XMFLOAT3(1.0f, 30.0f, 1.0f), DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f));

	for (const auto& trap : objects)
		trap->Initialize();
}

void TrapManager::Update(float elapsedTime)
{
	Manage(elapsedTime);
}