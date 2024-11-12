#pragma once


#include"Camera.h"
#include"FreeCameraController.h"
#include"Model.h"
#include"RenderContext.h"

namespace stage_number
{
	const int stage_max_num = 7;

	
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
	std::unique_ptr<Model>				stage_[stage_number::stage_max_num];
	int now_stage;

	DirectX::XMFLOAT4X4 stage_transform[stage_number::stage_max_num]
	{
		{
		0.0075, 0, 0, 0
		, 0, 0.0075, 0, 0
		, 0, 0, 0.0075, 0
		, 0, 5, 0, 1
		},
		{
		0.0075, 0, 0, 0
		, 0, 0.0075, 0, 0
		, 0, 0, 0.0075, 0
		, 0, 7.5, 0, 1
		},
		{
		0.0075, 0, 0, 0
		, 0, 0.0075, 0, 0
		, 0, 0, 0.0075, 0
		, 6.f, 5, 10.f, 1
		},
		{},
		{},
		{},
		{},
	};
};

