#include"stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"


#define collision_map false

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
}

