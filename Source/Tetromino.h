#pragma once

#include <type_traits>
#include <array>

namespace Tetromino
{
	enum TetrominoType
	{
		TETRO_T,
		TETRO_S,
		TETRO_Z,
		TETRO_L,
		TETRO_J,
	};

	template<TetrominoType T>
	struct TetrominoBlock : std::false_type
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_T> : std::true_type
	{
		static constexpr std::array<std::array<uint8_t, 3>, 2> rotate0 = {
			{
			{ 1, 1, 1},
			{ 0, 1, 0},
			},
		};

		static constexpr std::array<std::array<uint8_t, 2>, 3> rotate1 = {
			{
				{0, 1},
				{1, 1},
				{0, 1},
			},
		};

		static constexpr std::array<std::array<uint8_t, 3>, 2> rotate2 = {
			{
				{0, 1, 0},
				{1, 1, 1},
			}
		};

		static constexpr std::array<std::array<uint8_t, 2>, 3> rotate3 = {
			{
				{1, 0},
				{1, 1},
				{1, 0},
			}
		};
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_S> : std::true_type
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_Z> : std::true_type
	{
	};

#define Rotate(type, rotateIndex) ( TetrominoBlock<type>::rotate ## rotateIndex )

	class TetrominoCollider
	{
	public:
		TetrominoCollider() = default;

		template<TetrominoType Type>
		void PlaceTetromino(unsigned int top, unsigned int left, int rotate)
		{
			static_assert(TetrominoType<Type>::value == true, "This type is invalid");

			if (!DetectionCollide<Type>(x, y, rotate))
			{
				return;
			}

			auto shapeArray = Rotate(x, y, rotate);

			for (int row = 0; row < shapeArray.size(); ++row)
			{
				for (int col = 0; col < shapeArray[row].size(); ++col)
				{
					if (shapeArray[row][col] != 0)
					{
						Placed[row + top][col + left] = shapeArray[row][col];
					}
				}
			}


		}

		template<TetrominoType Type>
		bool DetectionCollide(unsigned int top, unsigned int left, unsigned int rotate)
		{
			static_assert(TetrominoType<Type>::value == true, "This type is invalid");

			auto shapeArray = Rotate(Type, rotate);

			for (int row = 0; row < shapeArray.size(); ++row)
			{
				for (int col = 0; col < shapeArray[row].size(); ++col)
				{
					if (shapeArray[row][col] != 0)
					{
						if (Placed[row + top][col + left] != 0)
						{
							return false;
						}
					}
				}
			}

			return true;
		}

	private:
		static constexpr unsigned int ROW_LENGTH = 9u;
		static constexpr unsigned int COL_LENGTH = 16u;
		int Placed[ROW_LENGTH][COL_LENGTH] = { 0 };


	};
}
