#include"stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"


#define collision_map false

Stage::Stage()
	:now_stage{0}
{
#if collision_map
	stage_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl");
	stage_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");
	stage_[2].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4_collision.mdl");
	stage_[0].transform = stage_collision_transform[0];
	stage_[1].transform = stage_collision_transform[1];
	stage_[2].transform = stage_collision_transform[2];
#else
	stage_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	stage_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
	stage_[2].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4.mdl");

	stage_[0].transform = stage_transform[0];
	stage_[1].transform = stage_transform[1];
	stage_[1].transform = stage_transform[2];
#endif

	stage_collision_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl");
	stage_collision_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");
	stage_collision_[2].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_4\\stage_4_collision.mdl");
	stage_collision_[0].transform = stage_collision_transform[0];
	stage_collision_[1].transform = stage_collision_transform[1];
	stage_collision_[2].transform = stage_collision_transform[2];

}

void Stage::Update(float elapsedTime)
{
	stage_[now_stage].UpdateTransform();
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
		, stage_[now_stage].transform
		, stage_[now_stage].model.get(), ShaderId::Lambert);

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

		ImGui::Spacing();
		{
			DirectX::XMFLOAT3 pos = stage_[now_stage].position;
			if (ImGui::InputFloat3("position", &pos.x))
			{
				stage_[now_stage].position = pos;
			}
		}
	}
	ImGui::End();
#endif
}

void Stage::SelectStage(int selector)
{
	if (selector < stage_number::stage_max_num)now_stage = selector;
}

