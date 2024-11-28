#pragma once
#include "Stage.h"

class Stage1 : public Stage
{
public:
	Stage1();
	~Stage1() override {};

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID);
};