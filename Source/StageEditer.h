#pragma once
#include <array>

namespace Stage
{
	enum StageNumber
	{
		Stage1,
		Stage2,
	};

	enum class TerrainBlockType
	{
		T1x1,
		T1x2,
		T1x3,
		T1x4,

		T2x1,
		T2x2,
		T2x3,
		T2x4,

		TNone,
	};


	template<TerrainBlockType>
	struct Terrain
	{
	};

	using TerrainArray = std::array<std::array<uint8_t, 6>, 3>;

	template<>
	struct Terrain<TerrainBlockType::T1x1>
	{
		static constexpr TerrainArray filed = { { {1}, {0}, {0} } };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x2>
	{
		static constexpr TerrainArray field = { 1, 1 };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x3>
	{
		static constexpr TerrainArray field = { 1, 1, 1 };
	};
	template<>
	struct Terrain<TerrainBlockType::T1x4>
	{
		static constexpr TerrainArray field = { 1, 1, 1, 1 };
	};

	struct TerrainData
	{
		TerrainBlockType type = TerrainBlockType::TNone;
		unsigned int topleft[2] = { 0, 0 };
	};
	template<StageNumber>
	struct StageData
	{
	};

	template<>
	struct StageData<StageNumber::Stage1>
	{
		static constexpr std::array<TerrainData, 1> data = {};
	};

	class StageTerrainCollision
	{
	private:
		static constexpr unsigned int ROW_LENGHT = 9u;
		static constexpr unsigned int COL_LENGHT = 16u;
		std::array<std::array<int, COL_LENGHT>, ROW_LENGHT> stagePlaced = {};
		StageNumber stageNumber;

	public:
		StageTerrainCollision() = default;

		void Initialize(StageNumber number);

		std::array<std::array<int, COL_LENGHT>, ROW_LENGHT> GetStagePlaced() { return stagePlaced; }
		auto GetStageData();
	};
}
