#pragma once
#include "Scene.h"
#include "Sprite.h"
#include <memory>

class SceneStageSelect : public Scene
{
public:
	SceneStageSelect();
	~SceneStageSelect(){}

	void Update(float elapsedTime) override;
	void Render(float elapsedTime) override;
	void DrawGUI() {};

private:
	std::unique_ptr<Sprite> StageSelect = nullptr;

	std::unique_ptr<Sprite> StageImage[6];
};