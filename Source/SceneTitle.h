#pragma once
#include "Scene.h"
#include "Sprite.h"
#include <memory>

class SceneTitle : public Scene
{
public:
	SceneTitle();
	~SceneTitle(){}

	void Update(float elapsedTime) override;
	void Render(float elapsedTime) override;
	void DrawGUI() {};

private:
	std::unique_ptr<Sprite> Title = nullptr;
};