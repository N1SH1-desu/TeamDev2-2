#pragma once
#include "Singleton.h"
#include "Manager.h"
#include "Key.h"

class KeyManager : public Singleton<KeyManager>, public Manager<Key>
{
	friend class Singleton<KeyManager>;
};