#pragma once
#include "Object.h"
#include "DirectXMath.h"

class Trap : public Object
{
	Object::Object;
public:
	virtual ~Trap() {};
	
	//初期化処理
	virtual void Initialize() = 0;

	//更新処理
	virtual void Update(float elapsedTime) = 0;
};