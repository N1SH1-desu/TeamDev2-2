#pragma once
#include "Object.h"
#include "DirectXMath.h"

class Trap : public Object
{
	Object::Object;
public:
	virtual ~Trap() {};
	
	//����������
	virtual void Initialize() = 0;

	//�X�V����
	virtual void Update(float elapsedTime) = 0;
};