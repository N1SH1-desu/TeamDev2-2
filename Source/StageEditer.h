#pragma once
#include <array>

namespace Stage
{
	enum class TerrainBlockType
	{
		T1x1,
		T1x2,
		T1x3,
		T1x4,

		T2x1,
		T2x2,
		T2x3,
		T2x4
	};

	template<TerrainBlockType>
	struct Terrain
	{};

	template<>
	struct Terrain<TerrainBlockType::T1x1>
	{
		static constexpr std::array<uint8_t, 1> filed = { 1 };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x2>
	{
		static constexpr std::array<uint8_t, 2> field = { 1, 1 };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x3>
	{
		static constexpr std::array<uint8_t, 3> field = { 1, 1, 1 };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x4>
	{
		static constexpr std::array<uint8_t, 4> field = { 1, 1, 1, 1 };
	};

	class StageTerrainCollision
	{
	public:
		StageTerrainCollision() = default;

	private:
		static constexpr unsigned int ROW_LENGHT = 9u;
		static constexpr unsigned int COL_LENGHT = 16u;
		int stagePlaced[ROW_LENGHT][COL_LENGHT] = { 0 };
	};
}
