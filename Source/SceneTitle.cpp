#include "SceneTitle.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "InputMouse.h"
#include "function.h"

enum
{
	NORMAL,
	HOVER
};

void SceneTitle::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	Title = std::make_unique<Sprite>(device, "./Data/Sprite/BackGround.jpg");
	PlayButton[NORMAL] = std::make_unique<Sprite>(device, "./Data/Sprite/PlayButton.png");
	PlayButton[HOVER] = std::make_unique<Sprite>(device, "./Data/Sprite/PlayButton_Hover.png");
	Interval = 0.0f;

	Name = std::make_unique<Sprite>(device, "./Data/Sprite/MinionsQuest.png");
}

void SceneTitle::Update(float elapsedTime)
{
	//this->audio_bgm_->Play(true);

	function& func = function::getInstance();
	InputMouse& input = InputMouse::Instance();

	Interval += elapsedTime;

	if (func.Click_Colision(DirectX::XMFLOAT2(550, 480), DirectX::XMFLOAT2(168, 88)) && input.IsLBottonDowned())
	{ 
		if (Interval > 0.5f) SceneManager::Instance().ChangeScene(new SceneStageSelect);
		//this->audio_bgm_->Stop();
	}
}

void SceneTitle::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();

	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Transparency), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);

	Title->Render(dc, -100, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
	Name->Render(dc, 0, 0, 0, 1280, 316, 0, 1, 1, 1, 1);

	function& func = function::getInstance();

	if(func.Click_Colision(DirectX::XMFLOAT2(550, 480), DirectX::XMFLOAT2(168, 88)))
		PlayButton[HOVER]->Render(dc, 550, 480, 0, 168, 88, 0, 1, 1, 1, 1);
	else PlayButton[NORMAL]->Render(dc, 550, 480, 0, 168, 88, 0, 1, 1, 1, 1);
}