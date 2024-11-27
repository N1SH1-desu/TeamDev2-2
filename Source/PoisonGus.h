#pragma once
#include "Trap.h"
class PoisonGus : public Trap
{
	Trap::Trap;

public:
	~PoisonGus() override {};

	void Initialize() override;
	void Update(float elapsedTime) override;

private:
	void Sensing(float elapsedTime);
};