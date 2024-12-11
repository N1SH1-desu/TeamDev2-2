#pragma once
#include <thread>
#include "Sprite.h"
#include "Scene.h"

static const int SMART_POINTER = 0;

class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {}

	//初期化
	void Initialize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(float elapsedTime) override;

	void Finalize() override;

private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:

#if SMART_POINTER
	std::unique_ptr<Scene> nextScene = nullptr;
	std::unique_ptr<std::thread> thread = nullptr;

#else
	Scene* nextScene = nullptr;
	std::unique_ptr<std::thread> thread = nullptr;
#endif

	std::unique_ptr<Sprite> NowLoading[4];

	float count = 0;
	int index = 0;
};