#include "stage.h"

#include<imgui.h>
#include<ImGuiRenderer.h>

#include"Graphics.h"
#include "TrapManager.h"
#include "Rock.h"
#include "Cage.h"
#include "PoisonGus.h"
#include "KeyManager.h"
#include "PortalManager.h"


Stage::Stage()
{
	stage_collision_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl");
	stage_collision_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");
	stage_collision_[2].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4_collision.mdl");
	stage_collision_[3].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_5\\stage_5_collision.mdl");
	stage_collision_[4].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6_collision.mdl");
	stage_collision_[5].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage.mdl");
	stage_collision_[6].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage_2.mdl");
	stage_collision_[0].transform = stage_collision_transform[0];
	stage_collision_[1].transform = stage_collision_transform[1];
	stage_collision_[2].transform = stage_collision_transform[2];
	stage_collision_[3].transform = stage_collision_transform[3];
	stage_collision_[4].transform = stage_collision_transform[4];
	stage_collision_[5].transform = stage_collision_transform[4];
	stage_collision_[6].transform = stage_collision_transform[4];

	SelectStage(5);
}

void Stage::Update(float elapsedTime)
{
	stage_.UpdateTransform();
}

void Stage::Render(float elapsedTime, RenderContext& rc)
{

	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	// ƒ‚ƒfƒ‹•`‰æ

	modelRenderer->Render(rc
		, stage_.transform
		, stage_.model.get(), ShaderId::Lambert);

}

void Stage::DrawGUI()
{
#if _DEBUG
	Graphics* graphics = &Graphics::Instance();

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(ImVec2(300, 720));
	float window_alpha = 0.25f;
	ImGui::SetNextWindowBgAlpha(window_alpha);

	if (ImGui::Begin(u8"stage", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		if (ImGui::Button("0"))SelectStage(0);
		if (ImGui::Button("1"))SelectStage(1);
		if (ImGui::Button("2"))SelectStage(2);
		if (ImGui::Button("3"))SelectStage(3);
		if (ImGui::Button("4"))SelectStage(4);
		if (ImGui::Button("5"))SelectStage(5);
		if (ImGui::Button("6"))SelectStage(6);

		ImGui::Spacing();
		{
			DirectX::XMFLOAT3 pos = stage_.position;
			if (ImGui::InputFloat3("position", &pos.x))
			{
				stage_.position = pos;
			}
		}
	}
	ImGui::End();
#endif
}

void Stage::SelectStage(int selector)
{
	if (selector > stage_number::stage_max_num)return;

	if (stage_.model.get() != nullptr)
	{
		stage_.model.reset(nullptr);
	}

	switch (selector)
	{
	case 0:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
		stage_.transform = stage_transform[0];
		break;
	case 1:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
		stage_.transform = stage_transform[1];
		break;
	case 2:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4.mdl");
		stage_.transform = stage_transform[2];
		break;
	case 3:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_5\\stage_5.mdl");
		stage_.transform = stage_transform[3];
		break;
	case 4:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6.mdl");
		stage_.transform = stage_transform[4];
		break;
	case 5:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage.mdl");
		stage_.transform = stage_transform[5];
		break;
	case 6:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage_2.mdl");
		stage_.transform = stage_transform[6];
		break;
	}

	ObjectSetting(selector);
}


void Stage::ObjectSetting(int selecter)
{
	//TrapManager::Instance().Clear();
	//KeyManager::Instance().Clear();
	//PortalManager::Instance().Clear();

	//switch (selecter)
	//{
	//case 0:
	//	TrapManager::Instance().Register(new PoisonGus("Data/Model/Floor.mdl", DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f)));
	//	KeyManager::Instance().Register(new Key("Data/Model/key.mdl", DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f)));
	//	PortalManager::Instance().Register(new Portal("Data/Model/portal.mdl", DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f)));
	//	break;
	//}
}