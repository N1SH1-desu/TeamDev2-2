#include "StageManager.h"
#include "Stage1.h"
#include "Stage2.h"
#include "imgui.h"


void StageManager::Initialize()
{
	Register(new Stage1);
}

void StageManager::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	for (const auto& stage : objects)
		stage->Render(modelRenderer, rc, ID);

	ImGui::Begin("Stage");
	if (ImGui::Button("Stage 1"))
	{
		Clear();
		Register(new Stage1);
	}

	if (ImGui::Button("Stage 2"))
	{
		Clear();
		Register(new Stage2);
	}

	ImGui::End();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult hit)
{
	//for (int i = 0; i < GetObjectCount(); i++)
	//{
	//	auto stage = GetObject_(i)->GetCollisionModel();

	//	if (Collision::RayCast(start, end, stage->GetTransform(), stage->GetModel(), hit.position, hit.normal))
	//		return true;
	//}

	return false;
}