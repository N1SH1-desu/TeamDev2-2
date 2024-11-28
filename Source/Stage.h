#pragma once
#include "Object.h"
#include <DirectXMath.h>

class Stage : public Object
{
public:
	Stage() {};
	virtual ~Stage() {};

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	Object* GetCollisionModel() { return CollisionModel.get(); }

protected:
	std::unique_ptr<Object> CollisionModel;
};