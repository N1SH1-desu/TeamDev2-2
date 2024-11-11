#pragma once


#include"Camera.h"
#include"FreeCameraController.h"
#include"Model.h"
#include"RenderContext.h"

namespace const_number
{
	const int stage_num = 7;
}

class Stage 
{
public:
	Stage(int selecter);
	~Stage() = default;

	void Update(float elapsedTime);

	// •`‰æˆ—
	void Render(float elapsedTime, RenderContext *rc);

	// GUI•`‰æˆ—
	void DrawGUI();

	void SelectStage(int selector);

private:
	std::unique_ptr<Model>				stage_[const_number::stage_num];
	int now_stage;


};

