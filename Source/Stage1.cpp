#include "Stage1.h"

Stage1::Stage1()
{
	Initialize();
}

void Stage1::Initialize()
{
	model = std::make_unique<Model>(".\\Data\\Model\\Stage\\stage_2\\stage_2.mdl");
	CollisionModel = std::make_unique<Object>(".\\Data\\Model\\Stage\\stage_2\\stage_2_collision.mdl", DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

	//stage
	position = { 0.0f, 7.5f, 0.0f };
}

void Stage1::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();

	CollisionModel->Update(elapsedTime);
}

void Stage1::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	modelRenderer->Render(rc, transform, model.get(), ID);
	CollisionModel->Render(modelRenderer, rc, ID);
}