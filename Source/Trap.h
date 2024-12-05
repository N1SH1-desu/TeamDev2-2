#pragma once
#include "Object.h"
#include "DirectXMath.h"

class Trap : public Object
{
	Object::Object;
public:
	virtual ~Trap() {};
	
	//‰Šú‰»ˆ—
	virtual void Initialize() = 0;

	//XVˆ—
	virtual void Update(float elapsedTime) = 0;
};