#include "SceneTitle.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"

SceneTitle::SceneTitle()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	Title = std::make_unique<Sprite>(device, "./Data/Sprite/Title.png");
}

void SceneTitle::Update(float elapsedTime)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x01) SceneManager::Instance().ChangeScene(new SceneStageSelect);
}

void SceneTitle::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	Title->Render(dc, 0, 0, 0, 1280, 720, 0, 1, 1, 1, 1);
}