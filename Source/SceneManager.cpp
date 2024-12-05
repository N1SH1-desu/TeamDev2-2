#include "SceneManager.h"

void SceneManager::Update(float elapsedTime, InputMouse* mouse)
{
	if (nextScene != nullptr)
	{
		Clear();
		currentScene = std::move(nextScene);
		nextScene = nullptr;
	}

	if (currentScene != nullptr)
	{
		currentScene->SetInputMouse(mouse);
		currentScene->Update(elapsedTime);
	}
}

void SceneManager::Render(float elapsedTime)
{
	if (currentScene != nullptr)
	{
		currentScene->Render(elapsedTime);
	}
}

void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene.reset();
		currentScene = nullptr;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	nextScene = std::unique_ptr<Scene>(scene);
}