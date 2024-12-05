#pragma once
#include "Singleton.h"
#include "Manager.h"
#include "Trap.h"

class TrapManager : public Manager<Trap>, public Singleton<TrapManager>
{
	//コンストラクタを"Singleton"が参照できるようにする
	friend class Singleton<TrapManager>; //https://cflat-inc.hatenablog.com/entry/2014/03/04/214608

private:
	TrapManager() {};
	~TrapManager() {};

public:
	void Initialize() override;
	void Update(float elapsedTime) override;

private:
	float Interval = 0.0f;
	float GenerateTime = 5.0f;
};