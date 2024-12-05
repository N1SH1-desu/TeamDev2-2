#pragma once
#include "Singleton.h"
#include "Manager.h"
#include "Portal.h"

class PortalManager : public Singleton<PortalManager>, public Manager<Portal>
{
	friend class Singleton<PortalManager>;
};