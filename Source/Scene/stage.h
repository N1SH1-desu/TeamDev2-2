#pragma once


#include"Scene.h"
#include"Camera.h"
#include"FreeCameraController.h"
#include"Model.h"


class Stage :public Scene
{
public:
	Stage();
	~Stage() override = default;

	void Update(float elapsedTime)override;

	// •`‰æˆ—
	void Render(float elapsedTime) override;

	// GUI•`‰æˆ—
	void DrawGUI() override;

private:
	Camera								camera;
	FreeCameraController				cameraController;

	std::unique_ptr<Model>				stage_1;

};

