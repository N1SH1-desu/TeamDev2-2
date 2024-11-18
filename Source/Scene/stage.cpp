#include"stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"

Stage::Stage(int selector)
	:now_stage{0}
{
	stage_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	stage_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
	stage_collision_[0].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl");
	stage_collision_[1].model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");

	stage_[0].transform = stage_transform[0];
	stage_[1].transform = stage_transform[1];
	stage_collision_[0].transform = stage_collision_transform[0];
	stage_collision_[1].transform = stage_collision_transform[1];
	stage_[0].model.get()->UpdateTransform();
	stage_[1].model.get()->UpdateTransform();
	stage_collision_[0].model.get()->UpdateTransform();
	stage_collision_[1].model.get()->UpdateTransform();

	SelectStage(selector);
}

void Stage::Update(float elapsedTime)
{
	stage_[now_stage].position =
	{
		stage_transform[now_stage]._41,
		stage_transform[now_stage]._42,
		stage_transform[now_stage]._43
	};
	stage_[now_stage].UpdateTransform();
}

void Stage::Render(float elapsedTime, RenderContext *rc)
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
	RenderContext myRc;
	myRc.camera = rc->camera;
	myRc.deviceContext = rc->deviceContext;
	myRc.renderState = rc->renderState;


	modelRenderer->Render(myRc
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

	}
	ImGui::End();
#endif
}

void Stage::SelectStage(int selector)
{
	if (selector < stage_number::stage_max_num)now_stage = selector;
}

