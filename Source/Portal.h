#pragma once
#include "Object.h"

class Portal : public Object
{
	using Object::Object;
public:
	void Update(float elapsedTime);
	bool Enabled();

private:
	bool enabled = false;
};