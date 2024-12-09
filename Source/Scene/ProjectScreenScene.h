#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include "FetchModelFromSceneAsset.h"
#include "Tetromino.h"
#include "KeyInput.h"
#include "TetroEditerUI.h"


class ProjectScreenScene : public Scene
{
public:
	ProjectScreenScene();
	~ProjectScreenScene() override = default;

	static ProjectScreenScene& Instance() {
		static ProjectScreenScene instance;
		return instance;
	}


	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render(float elapsedTime) override;

	// GUI•`‰æˆ—
	void DrawGUI() override;

private:
	struct Object
	{
		DirectX::XMFLOAT3		position = { 0, 0, 0 };
		DirectX::XMFLOAT3		angle = { 0, 0, 0 };
		DirectX::XMFLOAT3		scale = { 1, 1, 1 };
		DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		std::unique_ptr<Model>	model;
	};

	Camera								camera;
	FreeCameraController				cameraController;
	std::unique_ptr<Sprite>				sprite;
	Object								stage;
	std::vector<Object>					objs;
	std::unique_ptr<SceneModel>			sceneModels;
	Tetromino::TetrominoEditor			tetroEditer;
	
	int tetroType = 0;
	bool EditerMode = false;
	Input::KeyInput keyInput;

	TetroEditerUI editerUI;
};
