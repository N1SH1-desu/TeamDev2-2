#pragma once

namespace Tetromino
{
	enum class TetrominoType
	{
		TETRO_T,
		TETRO_S,
		TETRO_Z,
		TETRO_L,
		TETRO_J,
	};

	template<TetrominoType T>
	struct TetrominoBlock
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_T>
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_S>
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_Z>
	{
	};

	class TetrominoCollider
	{
	public:
		TetrominoCollider() = default;



	private:
		static constexpr unsigned int ROW_LENGTH = 9u;
		static constexpr unsigned int COL_LENGTH = 16u;
		int Placed[ROW_LENGTH][COL_LENGTH] = { 0 };


	};
}
