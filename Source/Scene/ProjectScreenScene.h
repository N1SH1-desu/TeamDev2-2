#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "FreeCameraController.h"
#include "Model.h"
#include "KeyInput.h"
#include "TetroEditerMode.h"
#include "StageEditer.h"
#include "AutoDrivenPlayer.h"


class ProjectScreenScene : public Scene
{
public:
	ProjectScreenScene();
	~ProjectScreenScene() override = default;

	static ProjectScreenScene& Instance() {
		static ProjectScreenScene instance;
		return instance;
	}


	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(float elapsedTime) override;

	// GUI描画処理
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
	Input::KeyInput keyInput;
	TetroEditerMode editerMode;

	AutoDrivenPlayer unityChan;
	TerrainStage::StageTerrain terrain;
};