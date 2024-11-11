#include"stage.h"

#include<imgui.h>
#include<ImGuizmo.h>

#include"Graphics.h"

Stage::Stage(int selector)
	:now_stage{0}
{
	stage_[0] = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_1\\stage_1.mdl");
	stage_[1] = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	stage_[2] = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");

	SelectStage(selector);
	
}

void Stage::Update(float elapsedTime)
{
	// カメラ更新処理


	stage_[now_stage].get()->UpdateTransform();
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

	modelRenderer->Render(myRc,
		{ 0.0075, 0, 0, 0
		, 0, 0.0075, 0, 0
		, 0, 0, 0.0075, 0
		, 6.f, 5, 10.f, 1 }
	, stage_[now_stage].get(), ShaderId::Lambert);

	
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
	}
	ImGui::End();
#endif
}

void Stage::SelectStage(int selector)
{
	if (selector < const_number::stage_num)now_stage = selector;

	//switch (selector)
	//{
	//case 1:
	//	stage_ = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_1\\stage_1.mdl");
	//	break;
	//case 2:
	//	stage_ = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	//	break;
	//case 3:
	//	stage_ = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
	//	break;
	//}
}
