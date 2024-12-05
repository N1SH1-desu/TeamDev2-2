#include "stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"
#include "TrapManager.h"
#include "Rock.h"
#include "Cage.h"
#include "PoisonGus.h"
#include "KeyManager.h"
#include "PortalManager.h"

#define collision_map false
const char* StageModelPath[]
{
	"./Data/Model/Stage/stage_2/stage_2.mdl",
	"./Data/Model/Stage/stage_3/stage_3.mdl",
	"./Data/Model/Stage/stage_4/stage_4.mdl",
	"./Data/Model/Stage/stage_5/stage_5.mdl",
	"./Data/Model/Stage/stage_6/stage_6.mdl",
	"./Data/Model/Stage/stage_test/test_stage.mdl",
	"./Data/Model/Stage/stage_test/test_stage_2.mdl"
};

const char* CollisionModelPath[]
{
	"./Data/Model/Stage/stage_2/stage_2_collision.mdl",
	"./Data/Model/Stage/stage_3/stage_3_collision.mdl",
	"./Data/Model/Stage/stage_4/stage_4_collision.mdl",
	"./Data/Model/Stage/stage_5/stage_5_collision.mdl",
	"./Data/Model/Stage/stage_6/stage_6_collision.mdl",
	"./Data/Model/Stage/stage_test/test_stage.mdl",
	"./Data/Model/Stage/stage_test/test_stage.mdl"
};

Stage::Stage()

{
#if collision_map
	stage_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl");
	stage_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");
	stage_[2].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4_collision.mdl");
	stage_[3].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_5\\stage_5_collision.mdl");
	stage_[4].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6_collision.mdl");
	stage_[5].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6_collision.mdl");
	stage_[6].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6_collision.mdl");
	stage_[0].transform = stage_collision_transform[0];
	stage_[1].transform = stage_collision_transform[1];
	stage_[2].transform = stage_collision_transform[2];
	stage_[3].transform = stage_collision_transform[3];
	stage_[4].transform = stage_collision_transform[4];
	stage_[5].transform = stage_collision_transform[4];
	stage_[6].transform = stage_collision_transform[4];
#else
	stage_.transform = stage_transform[0];

#endif
};


void Stage::Update(float elapsedTime)
{
	stage_.UpdateTransform();

	switch (now_stage)
	{
	case 4:
		static float Interval = 0.0f;
		Interval += elapsedTime;

		if (Interval > 3.0f)
		{
			TrapManager::Instance().Register(new Rock("./Data/Model/Rock.mdl", DirectX::XMFLOAT3(1.0f, 10.0f, 1.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f));
			Interval = 0.0f;
		}
		break;
	}
}

void Stage::Render(float elapsedTime, RenderContext &rc)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	// レンダーステート設定
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	// モデル描画

	modelRenderer->Render(rc
		, stage_.transform
		, stage_.model.get(), ShaderId::Lambert);

	DrawGUI();
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
		if (ImGui::Button("0"))
		{
			SelectStage(0);
		}

		if (ImGui::Button("1"))
		{
			SelectStage(1);
		}
		if (ImGui::Button("2"))
		{
			SelectStage(2);
		}
		if (ImGui::Button("3"))
		{
			SelectStage(3);
		}
		if (ImGui::Button("4"))
		{
			TrapManager::Instance().Clear();
			SelectStage(4);
		}
		if (ImGui::Button("5"))
		{
			SelectStage(5);
		}
		if (ImGui::Button("6"))
		{
			SelectStage(6);
		}

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

	stage_.model.release();
	stage_.model = nullptr;

	switch (selector)
	{
	case 0:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
		break;
	case 1:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
		break;
	case 2:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4.mdl");
		break;
	case 3:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_5\\stage_5.mdl");
		break;
	case 4:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_6\\stage_6.mdl");
		break;
	case 5:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage.mdl");
		break;
	case 6:
		stage_.model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_test\\test_stage_2.mdl");
		break;
	}

	ObjectSetting(selector);

	stage_collision_[selector].model = std::make_unique<Model>(CollisionModelPath[selector]);
}

void Stage::ObjectSetting(int selecter)
{
	TrapManager::Instance().Clear();
	KeyManager::Instance().Clear();
	PortalManager::Instance().Clear();

	switch (selecter)
	{
	case 0:
		TrapManager::Instance().Register(new PoisonGus("Data/Model/Floor.mdl", DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f)));
		KeyManager::Instance().Register(new Key("Data/Model/key.mdl", DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f)));
		PortalManager::Instance().Register(new Portal("Data/Model/portal.mdl", DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f)));
		break;
	}
}