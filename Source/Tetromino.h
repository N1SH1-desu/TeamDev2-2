#pragma once

#include <type_traits>
#include <array>

namespace Tetromino
{
	using TetroArray = std::array<std::array<uint8_t, 3>, 3>;

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
		static constexpr TetroArray rotate0 = {
			{
				{ 1, 1, 1},
				{ 0, 1, 0},
				{ 0, 0, 0},
			},
		};
		static constexpr TetroArray rotate1 = {
			{
				{0, 1, 0},
				{1, 1, 0},
				{0, 1, 0},
			},
		};
		static constexpr TetroArray rotate2 = {
			{
				{0, 1, 0},
				{1, 1, 1},
				{0, 0, 0},
			}
		};
		static constexpr TetroArray rotate3 = {
			{
				{1, 0, 0},
				{1, 1, 0},
				{1, 0, 0},
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

#define CheckRotate(type, rotateIndex)\
switch (rotate)\
{\
			case 0:\
				shapeArray = Rotate(Type, 0);\
				break;\
			case 1:\
				shapeArray = Rotate(Type, 1);\
				break;\
			case 2:\
				shapeArray = Rotate(Type, 2);\
				break;\
			case 3:\
				shapeArray = Rotate(Type, 3);\
				break;\
}
		

	class TetrominoCollider
	{
	public:
		TetrominoCollider() = default;

		template<TetrominoType Type>
		void PlaceTetromino(unsigned int top, unsigned int left, int rotate)
		{
			static_assert(TetrominoBlock<Type>::value == true, "This type is invalid");

			if (!DetectionCollide<Type>(x, y, rotate))
			{
				return;
			}

			TetroArray shapeArray{};
			CheckRotate(Type, rotate);

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
			static_assert(TetrominoBlock<Type>::value == true, "This type is invalid");

			TetroArray shapeArray{};
			CheckRotate(Type, rotate);
			
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
