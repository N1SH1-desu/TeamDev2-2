#pragma once
#include "Object.h"

class Key : public Object
{
	using Object::Object;

public:
	void Update(float elapsedTime) override;
	void Behavior(float elapsedTime);
	void DrawGUI() override;

private:
	float TurnSpeed = 1.0f;
	float VerticalSpeed = 0.75f;
	float Interval = 0.0f;
	float magnitication = 1.0f;
};