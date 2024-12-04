#pragma once
#include "Trap.h"
class PoisonGus : public Trap
{
	Trap::Trap;

public:
	~PoisonGus() override {};

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID) override;

private:
	void Sensing(float elapsedTime);
};