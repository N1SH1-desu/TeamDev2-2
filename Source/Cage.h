#pragma once
#include "Trap.h"
class Cage : public Trap
{
	Trap::Trap;

public:
	Cage();
	~Cage() override {};

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(ModelRenderer* modelRenderer, RenderContext& rc,  ShaderId ID);

private:
	void Sensing(float elapsedTime);
	void UpdateVerticalMove(float elapsedTime);

private:
	std::unique_ptr<Object> Floor;

	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	float gravity = -20.0f;

	bool fall = false;

	float durability = 10.0f;
};