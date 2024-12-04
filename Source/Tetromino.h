#pragma once

#include <type_traits>
#include <array>
#include <vector>
#include <DirectXMath.h>

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
		TETRO_I,
		TETRO_O
	};

	template<TetrominoType T>
	struct TetrominoBlock : std::false_type
	{
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_T> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{1, 1, 1},
					{0, 1, 0},
					{0, 0, 0}
				}},
				{{
					{0, 1, 0},
					{1, 1, 0},
					{0, 1, 0}
				}},
				{{
					{0, 1, 0},
					{1, 1, 1},
					{0, 0, 0}
				}},
				{{
					{1, 0, 0},
					{1, 1, 0},
					{1, 0, 0}
				}},
			}
		};
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_S> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{0, 1, 1},
					{1, 1, 0},
					{0, 0, 0}
				}},
				{{
					{1, 0, 0},
					{1, 1, 0},
					{0, 1, 0}
				}},
				{{
					{0, 1, 1},
					{1, 1, 0},
					{0, 0, 0},
				}},
				{{
					{1, 0, 0},
					{1, 1, 0},
					{0, 1, 0},
				}}
			}
		};
	};

	template <>
	struct TetrominoBlock<TetrominoType::TETRO_Z> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{1, 1, 0},
					{0, 1, 1},
					{0, 0, 0}
				}},
				{{
					{0, 1, 0},
					{1, 1, 0},
					{1, 0, 0}
				}},
				{{
					{1, 1, 0},
					{0, 1, 1},
					{0, 0, 0},
				}},
				{{
					{0, 1, 0},
					{1, 1, 0},
					{1, 0, 0},
				}}
			}
		};
	};
	
	template<>
	struct TetrominoBlock<TetrominoType::TETRO_L> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{1, 0, 0},
					{1, 0, 0},
					{1, 1, 0}
				}},
				{{
					{1, 1, 1},
					{1, 0, 0},
					{0, 0, 0}
				}},
				{{
					{1, 1, 0},
					{0, 1, 0},
					{0, 1, 0},
				}},
				{{
					{0, 0, 1},
					{1, 1, 1},
					{0, 0, 0},
				}}
			}
		};
	};

	template<>
	struct TetrominoBlock<TetrominoType::TETRO_J> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{0, 1, 0},
					{0, 1, 0},
					{1, 1, 0}
				}},
				{{
					{1, 0, 0},
					{1, 1, 1},
					{0, 0, 0}
				}},
				{{
					{1, 1, 0},
					{1, 0, 0},
					{1, 0, 0},
				}},
				{{
					{1, 1, 1},
					{0, 0, 1},
					{0, 0, 0},
				}}
			}
		};
	};

	template<>
	struct TetrominoBlock<TetrominoType::TETRO_I> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{1, 0, 0},
					{1, 0, 0},
					{1, 0, 0}
				}},
				{{
					{1, 1, 1},
					{0, 0, 0},
					{0, 0, 0}
				}},
				{{
					{1, 0, 0},
					{1, 0, 0},
					{1, 0, 0},
				}},
				{{
					{1, 1, 1},
					{0, 0, 0},
					{0, 0, 0},
				}}
			}
		};
	};

	template<>
	struct TetrominoBlock<TetrominoType::TETRO_O> : std::true_type
	{
		static constexpr std::array<TetroArray, 4> rotations = {
			{
				{{
					{1, 1, 0},
					{1, 1, 0},
					{0, 0, 0}
				}},
				{{
					{1, 1, 0},
					{1, 1, 0},
					{0, 0, 0}
				}},
				{{
					{1, 1, 0},
					{1, 1, 0},
					{0, 0, 0},
				}},
				{{
					{1, 1, 0},
					{1, 1, 0},
					{0, 0, 0},
				}}
			}
		};
	};

#define CheckRotate(type, rotateIndex) shapeArray = TetrominoBlock<type>::rotations[rotateIndex];
		

	class TetrominoCollider
	{
	public:
		TetrominoCollider() = default;

		template<TetrominoType Type>
		bool PlaceTetromino(unsigned int top, unsigned int left, int rotate)
		{
			static_assert(TetrominoBlock<Type>::value == true, "This type is invalid");

			if (!DetectionCollide<Type>(top, left, rotate))
			{
				return false;
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
			return true;

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
						if (Placed[row + top][col + left] != 0 || row + top >= ROW_LENGTH || col + left >= COL_LENGTH)
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

	class TetroRenderer
	{
	public:
		TetroRenderer() = default;

		template<TetrominoType Type>
		void CalcWorldPosition(unsigned int top, unsigned int left, unsigned int rotate,
			const unsigned int offset = 8.0f, const float xAxisMax = 60.0f, const float yAxisMax = 32.0f)
		{
			TetroArray shapeArray{};
			CheckRotate(Type, rotate);

			worldPositions.clear();

			for (int row = 0; row < shapeArray.size(); row++)
			{
				for (int col = 0; col < shapeArray[row].size(); col++)
				{
					if (shapeArray[row][col] != 0)
					{
						float x = -xAxisMax + ((left + col) * offset);
						if (x >= xAxisMax) x = xAxisMax;

						float y = yAxisMax - ((top + row) * offset);
						if (y <= -yAxisMax) y = -yAxisMax;

						worldPositions.push_back({ x, y });
					}
				}
			}
		}

		void UpdateTransform(DirectX::XMFLOAT3 scale)
		{
			transforms.clear();
			for (DirectX::XMFLOAT2& wp : worldPositions)
			{
				DirectX::XMFLOAT4X4 st;
				DirectX::XMStoreFloat4x4(&st, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(wp.x, wp.y, 0.0f));
				transforms.push_back(st);
			}
		}

		const std::vector<DirectX::XMFLOAT4X4>& GetTransforms() const { return transforms; }

	private:
		std::vector<DirectX::XMFLOAT4X4> transforms;
		std::vector<DirectX::XMFLOAT2> worldPositions;
	};
}
