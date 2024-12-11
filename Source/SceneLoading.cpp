#include "SceneLoading.h"
#include "SceneManager.h"
#include "Graphics.h"

const char* filePath_[]
{
	"./Data/Sprite/NowLoading0.png",
	"./Data/Sprite/NowLoading1.png",
	"./Data/Sprite/NowLoading2.png",
	"./Data/Sprite/NowLoading3.png",
};

void SceneLoading::Initialize()
{
	ID3D11Device* dc = Graphics::Instance().GetDevice();
	thread = std::make_unique<std::thread>(LoadingThread, this);

	for (int i = 0; i < 4; i++) 
		NowLoading[i] = std::make_unique<Sprite>(dc, filePath_[i]);

	index = 0;
	count = 0;
}

void SceneLoading::Update(float elapsedTime)
{
	if (nextScene->IsReady())
		SceneManager::Instance().ChangeScene(nextScene);

	count += elapsedTime;

	if (count > 0.3f)
	{
		index++;
		count = 0.0f;

		if (index > 3) index = 0;
	}
}

void SceneLoading::Render(float elpasedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();

	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Transparency), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);

	NowLoading[index]->Render(dc, 643, 601, 0, 643, 119, 0, 1, 1, 1, 1);
}

void SceneLoading::Finalize()
{
	thread->detach();
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	//���̃V�[���̏�����
	scene->nextScene->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
}