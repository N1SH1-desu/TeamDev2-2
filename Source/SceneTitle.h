#pragma once
#include "Scene.h"
#include "Sprite.h"
#include <memory>

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle(){}

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(float elapsedTime) override;
	void Finalize() override {};
	void DrawGUI() {};

private:
	std::unique_ptr<Sprite> Title = nullptr;
	std::unique_ptr<Sprite> PlayButton[2];

	float Interval = 0.0f;
};