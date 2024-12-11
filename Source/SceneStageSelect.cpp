#include "SceneStageSelect.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Scene/AnimationScene.h"
#include "InputMouse.h"
#include "function.h"
#include "SceneLoading.h"

static const int BUTTON_SIZE = 120;
static const int DISPLAY_MARGIN = 240;

const char* filePath[]
{
	"./Data/Sprite/Stage1.png",
	"./Data/Sprite/Stage2.png",
	"./Data/Sprite/Stage3.png",
	"./Data/Sprite/Stage4.png",
	"./Data/Sprite/Stage5.png",
	"./Data/Sprite/Stage6.png",
};

const char* filePath_Hover[]
{
	"./Data/Sprite/Stage1_Hover.png",
	"./Data/Sprite/Stage2_Hover.png",
	"./Data/Sprite/Stage3_Hover.png",
	"./Data/Sprite/Stage4_Hover.png",
	"./Data/Sprite/Stage5_Hover.png",
	"./Data/Sprite/Stage6_Hover.png",
};

void SceneStageSelect::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	BackGround = std::make_unique<Sprite>(device, "./Data/Sprite/BackGround.jpg");
	StageSelect = std::make_unique<Sprite>(device, "./Data/Sprite/StageSelect.png");

	for (int i = 0; i < 6; i++)
	{
		StageImage[i] = std::make_unique<Sprite>(device, filePath[i]);
		StageImage_Hover[i] = std::make_unique<Sprite>(device, filePath_Hover[i]);
	}

	this->bgm_ = Audio::Instance().LoadAudioSource("./Data/Audio/select.wav");

	Interval = 0.0f;
}

void SceneStageSelect::Update(float elapsedTime)
{
	function& func = function::getInstance();
	InputMouse& mouse = InputMouse::Instance();

	this->bgm_->Play(true);

	for (int i = 0; i < 3; i++)
	{
		//クリック処理
		if (Interval > 0.5f)
			if (func.Click_Colision(DirectX::XMFLOAT2((i * DISPLAY_MARGIN) + 360, 300), DirectX::XMFLOAT2(BUTTON_SIZE, BUTTON_SIZE)) && mouse.IsLBottonDowned())
				SceneManager::Instance().ChangeScene(new SceneLoading(new AnimationScene(i)));

		//クリック処理
		if (Interval > 0.5f)
			if (func.Click_Colision(DirectX::XMFLOAT2((i * DISPLAY_MARGIN) + 360, 500), DirectX::XMFLOAT2(BUTTON_SIZE, BUTTON_SIZE)) && mouse.IsLBottonDowned())
				SceneManager::Instance().ChangeScene(new SceneLoading(new AnimationScene(i + 3)));
	}
}

void SceneStageSelect::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Transparency), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);

	function& func = function::getInstance();

	Interval += elapsedTime;

	BackGround->Render(dc, -100, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
	StageSelect->Render(dc, 300, 0, 0, 720, 128, 0, 1, 1, 1, 1);

	for (int i = 0; i < 3; i++)
	{
		//画像の表示と切り替え
		if (func.Click_Colision(DirectX::XMFLOAT2((i * DISPLAY_MARGIN) + 360, 300), DirectX::XMFLOAT2(BUTTON_SIZE, BUTTON_SIZE)))
			StageImage_Hover[i]->Render(dc, (i * DISPLAY_MARGIN) + 360, 300, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 1, 1, 1, 1);
		else  StageImage[i]->Render(dc, (i * 240) + 360, 300, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 1, 1, 1, 1);

		if (func.Click_Colision(DirectX::XMFLOAT2((i * DISPLAY_MARGIN) + 360, 500), DirectX::XMFLOAT2(BUTTON_SIZE, BUTTON_SIZE)))
			StageImage_Hover[i + 3]->Render(dc, (i * DISPLAY_MARGIN) + 360, 500, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 1, 1, 1, 1);
		else StageImage[i + 3]->Render(dc, (i * DISPLAY_MARGIN) + 360, 500, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 1, 1, 1, 1);
	}
}