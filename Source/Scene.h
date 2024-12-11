#pragma once
#include "InputMouse.h"
#include "Audio/Audio.h"


// �V�[�����
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Initialize(){}

	// �X�V����
	virtual void Update(float elapsedTime) {}

	// �`�揈��
	virtual void Render(float elapsedTime) {}

	virtual void Finalize(){}

	// GUI�`�揈��
	virtual void DrawGUI() {}

	bool IsReady() const { return ready; }
	
	void SetReady() { ready = true; }
protected:
	InputMouse* refInputMouse;
	bool ready = false;

};
