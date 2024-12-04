#include "SceneStageSelect.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Scene/AnimationScene.h"

const char* filePath[]
{
	"./Data/Sprite/Stage1.png",
	"./Data/Sprite/Stage2.png",
	"./Data/Sprite/Stage3.png",
	"./Data/Sprite/Stage4.png",
	"./Data/Sprite/Stage5.png",
	"./Data/Sprite/Stage6.png",
};

SceneStageSelect::SceneStageSelect()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	StageSelect = std::make_unique<Sprite>(device, "./Data/Sprite/StageSelect.png");

	for (int i = 0; i < 6; i++) 
		StageImage[i] = std::make_unique<Sprite>(device, filePath[i]);
}

void SceneStageSelect::Update(float elapsedTime)
{
	if (GetAsyncKeyState('1')) SceneManager::Instance().ChangeScene(new AnimationScene(0));
	if (GetAsyncKeyState('2')) SceneManager::Instance().ChangeScene(new AnimationScene(1));
	if (GetAsyncKeyState('3')) SceneManager::Instance().ChangeScene(new AnimationScene(2));
	if (GetAsyncKeyState('4')) SceneManager::Instance().ChangeScene(new AnimationScene(3));
	if (GetAsyncKeyState('5')) SceneManager::Instance().ChangeScene(new AnimationScene(4));
	if (GetAsyncKeyState('6')) SceneManager::Instance().ChangeScene(new AnimationScene(5));
}

void SceneStageSelect::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	for (int i = 0; i < 3; i++)
	{
		StageImage[i]->Render(dc, (i * 240) + 240, 120, 0, 240, 240, 0, 1, 1, 1, 1);
		StageImage[i + 3]->Render(dc, (i * 240) + 240, 120 + 240, 0, 240, 240, 0, 1, 1, 1, 1);
	}

	StageSelect->Render(dc, 0, 0, 0, 1280, 720, 0, 1, 1, 1, 1);
}