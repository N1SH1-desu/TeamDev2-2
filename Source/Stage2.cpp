#include "Stage2.h"

Stage2::Stage2()
{
	Initialize();
}

void Stage2::Initialize()
{
	model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_3\\stage_3.mdl");
	CollisionModel = std::make_unique<Object>(".\\Data\\Model\\Stage\\stage_3\\stage_3_collision.mdl");

	position = { 0.0f, 7.5f, 0.0f };
}

void Stage2::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();

	CollisionModel->Update(elapsedTime);
}

void Stage2::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	modelRenderer->Render(rc, transform, model.get(), ID);
	CollisionModel->Render(modelRenderer, rc, ID);
}