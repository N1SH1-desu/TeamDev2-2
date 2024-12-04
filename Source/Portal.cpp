#include "Portal.h"
#include "KeyManager.h"

void Portal::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();
}

bool Portal::Enabled()
{
	if (!KeyManager::Instance().GetObjectCount()) return true;

	return false;
}