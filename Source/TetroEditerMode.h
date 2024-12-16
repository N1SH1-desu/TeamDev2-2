#pragma once
#include "Graphics.h"
#include "KeyInput.h"
#include "Tetromino.h"
#include "TetroEditerUI.h"
#include "Grid2DRenderer.h"
#include "StageEditer.h"

class TetroEditerMode
{
public:
	TetroEditerMode() = default;

	const SceneModel* GetSceneModel() const { return tetroBlockModels.get(); }

	Tetromino::TetrominoCollider::TetroCollideArray GetTetroCollideArray() { return tetroEditer.GetCollider().GetTetroCollidePlaced(); }

	void Initialize(ID3D11Device* device, ID2D1DeviceContext* d2dContext);
	void Update(float elapsedTime, POINTS mousePos, const Input::KeyInput& keyInput, const TerrainStage::StageTerrain::StageArray& stageCollision);
	void Render(RenderContext rc, ID2D1DeviceContext* d2dContext, ModelRenderer* mRenderer);

private:
	Tetromino::TetrominoEditor tetroEditer;
	TetroEditerUI editerUI;
	std::unique_ptr<Grid2DRenderer>	gridRenderer;

	std::shared_ptr<SceneModel> tetroBlockModels;

	bool EditerMode = false;
};