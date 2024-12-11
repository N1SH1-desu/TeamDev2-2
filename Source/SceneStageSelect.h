#pragma once
#include "Scene.h"
#include "Sprite.h"
#include <memory>

class SceneStageSelect : public Scene
{
public:
	~SceneStageSelect(){}

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(float elapsedTime) override;
	void Finalize() override {};
	void DrawGUI() {};

private:
	std::unique_ptr<Sprite> BackGround = nullptr;
	std::unique_ptr<Sprite> StageSelect = nullptr;
	std::unique_ptr<Sprite> StageImage[6];
	std::unique_ptr<Sprite> StageImage_Hover[6];

	float Interval = 0.0f;

	std::unique_ptr<AudioSource>	bgm_;
};