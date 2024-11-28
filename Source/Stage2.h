#pragma once
#include "Stage.h"

class Stage2 : public Stage
{
public:
	Stage2();
	~Stage2() override {};

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID);
};