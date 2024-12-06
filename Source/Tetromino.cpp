#include "Tetromino.h"

namespace Tetromino
{
    void CheckRotate(TetroArray& shapeArray, TetrominoType type, int rotate)
    {
		switch (type)
		{
		case Tetromino::TETRO_T:
			shapeArray = TetrominoBlock<TETRO_T>::rotations[rotate];
			break;
		case Tetromino::TETRO_S:
			shapeArray = TetrominoBlock<TETRO_S>::rotations[rotate];
			break;
		case Tetromino::TETRO_Z:
			shapeArray = TetrominoBlock<TETRO_Z>::rotations[rotate];
			break;
		case Tetromino::TETRO_L:
			shapeArray = TetrominoBlock<TETRO_L>::rotations[rotate];
			break;
		case Tetromino::TETRO_J:
			shapeArray = TetrominoBlock<TETRO_J>::rotations[rotate];
			break;
		case Tetromino::TETRO_I:
			shapeArray = TetrominoBlock<TETRO_I>::rotations[rotate];
			break;
		case Tetromino::TETRO_O:
			shapeArray = TetrominoBlock<TETRO_O>::rotations[rotate];
			break;
		}
    }

	void TetroRenderer::RenderSelcetedTetromino(RenderContext& rc, ModelRenderer* mR, ShaderId id, bool ortho)
	{
		for (const DirectX::XMFLOAT4X4& tf : transforms)
		{
			sceneModel->SelectedBlockRender(rc, mR, tf, colorIndex - 1, ShaderId::Lambert, true);
		}
	}
	void TetroRenderer::RenderCommitedTetromino(RenderContext& rc, ModelRenderer* mR, ShaderId id, bool ortho)
	{
		sceneModel->RenderCommitedBlocks(rc, mR, ShaderId::Lambert, true);
	}



	void TetrominoEditor::Update(const POINTS mousePos, const Input::KeyInput keyFiled, SceneModel* sceneModels)
	{
		static int x = 0;
		static int y = 0;
		static int rotate = 0;
		static int tetroType = 0;

		if (keyFiled.GetKeyStatus('F') == Input::KeyStatus::Release)
		{
			tetroType++;
			if (tetroType > 6)
				tetroType = 0;
		}

		if (keyFiled.GetKeyStatus('R') == Input::KeyStatus::Release)
		{
			rotate++;
			if (rotate > 3)
				rotate = 0;
		}

		int xGrid = mousePos.x / static_cast<SHORT>(Grid2DRenderer::grid_size);
		int yGrid = mousePos.y / static_cast<SHORT>(Grid2DRenderer::grid_size);

		if (collider.DetectionCollide(static_cast<Tetromino::TetrominoType>(tetroType), yGrid, xGrid, rotate))
		{
			x = xGrid;
			y = yGrid;
		}

		int colorIndex = renderer.CalcWorldPosition(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate);
		constexpr float SCALE = 8.0f;
		renderer.UpdateTransform(DirectX::XMFLOAT3{ SCALE, SCALE, SCALE });

		if (keyFiled.GetKeyStatus(VK_SPACE) == Input::Release)
		{
			if (collider.PlaceTetromino(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate))
			{
				auto transforms = renderer.GetTransforms();
				for (DirectX::XMFLOAT4X4 ts : transforms)
				{
					sceneModels->CommitBlock({ (colorIndex - 1), std::move(ts) });
				}
			}
		}

		renderer.AttackSceneModel(sceneModels);

	}

	void TetrominoEditor::Render(RenderContext& rc, ModelRenderer* mR)
	{
		renderer.RenderSelcetedTetromino(rc, mR, ShaderId::Lambert, true);
		renderer.RenderCommitedTetromino(rc, mR, ShaderId::Lambert, true);
	}

}
