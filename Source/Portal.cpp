#include "Portal.h"
#include "KeyManager.h"

void Portal::Update(float elapsedTime)
{
	radius = 15.0f;
	height = 15.0f;

	UpdateTransform();
	model->UpdateTransform();
}

bool Portal::Enabled()
{
	if (KeyManager::Instance().GetObjectCount() < 1) return true;

	return false;
}