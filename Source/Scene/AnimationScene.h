#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include "player.h"
#include "stage.h"
#include "FetchModelFromSceneAsset.h"
#include "stage.h"
#include "KeyInput.h"
#include "TetroEditerMode.h"

class AnimationScene : public Scene
{
public:
	AnimationScene(int StageNum);
	~AnimationScene();

	void Initialize() override;
	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render(float elapsedTime) override;


	// GUI•`‰æˆ—
	//void DrawGUI() override;


private:
	std::unique_ptr<Player> player = nullptr;

	FreeCameraController				cameraController;

	struct Object
	{
		DirectX::XMFLOAT3		position = { 0, 0, 0 };
		DirectX::XMFLOAT3		angle = { 0, 0, 0 };
		DirectX::XMFLOAT3		scale = { 1, 1, 1 };
		DirectX::XMFLOAT4X4		transform = { 
			1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			0, 0, 0, 1 };
		std::unique_ptr<Model>	model;

		void UpdateTransform()
		{
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			DirectX::XMMATRIX WorldTransform = S * R * T;
			DirectX::XMStoreFloat4x4(&transform, WorldTransform);
			model->UpdateTransform();
		}
	};
	Object								cube;
	Object								cube2;

	//std::unique_ptr<Stage> stage = nullptr;


	float									timer = 0;
	int										Co = 0;

	std::unique_ptr<Stage> stage = nullptr;

	int StageNumber = 0;

	Input::KeyInput keyinput;
	TetroEditerMode EditerMode;
};
