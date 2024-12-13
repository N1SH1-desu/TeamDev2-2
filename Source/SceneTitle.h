#pragma once
#include <memory>

#include "Scene.h"
#include "Sprite.h"
#include"Audio/Audio.h"

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
	std::unique_ptr<Sprite> Name = nullptr;

	float Interval = 0.0f;

	std::unique_ptr<AudioSource> audio_bgm_;
};