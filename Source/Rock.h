#pragma once
#include "Trap.h"

class Rock : public Trap
{
	Trap::Trap;
public:
	~Rock() override{};

	void Initialize() override;
	void Update(float elapsedTime) override;

private:
	void UpdateVerticalMove(float elapsedTime);

private:
	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	float gravity = -30.0f;
};