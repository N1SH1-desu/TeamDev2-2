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
}
