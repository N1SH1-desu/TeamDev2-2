#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		Clear();
		currentScene = std::move(nextScene);
		nextScene = nullptr;

		if (!currentScene->IsReady())
			currentScene->Initialize();
	}

	if (currentScene != nullptr)
	{
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
		currentScene->Finalize();
		currentScene.reset();
		currentScene = nullptr;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	nextScene = std::unique_ptr<Scene>(scene);
}