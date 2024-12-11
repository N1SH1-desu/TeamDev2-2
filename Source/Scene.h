#pragma once
#include "InputMouse.h"
#include "Audio/Audio.h"


// シーン基底
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Initialize(){}

	// 更新処理
	virtual void Update(float elapsedTime) {}

	// 描画処理
	virtual void Render(float elapsedTime) {}

	virtual void Finalize(){}

	// GUI描画処理
	virtual void DrawGUI() {}

	bool IsReady() const { return ready; }
	
	void SetReady() { ready = true; }
protected:
	InputMouse* refInputMouse;
	bool ready = false;

};
