#pragma once
#include <array>
#include "FetchModelFromSceneAsset.h"

namespace Stage
{
	enum StageNumber
	{
		Stage1,
		Stage2,
	};

	enum class TerrainBlockType
	{
		GrassSingle,
		Grass1x2,
		Grass1x3,
		Grass1x4,

		DirtSingle,
		Dirt1x2,
		Dirt1x3,
		Dirt1x4,

		TNone,
	};

	enum TerrainElementType
	{
		TET_GrassLC = 0x1,
		TET_GrassRC,
		TET_GrassMid,

		TET_DirtLC,
		TET_DirtRC,
		TET_DirtMid,

		TET_GrassSingle,
		TET_DirtSingle,

		TET_None = 0x0,
	};


	template<TerrainBlockType>
	struct Terrain
	{
	};

	using TerrainArray = std::array<TerrainElementType, 6>;

	template<>
	struct Terrain<TerrainBlockType::GrassSingle>
	{
		static constexpr TerrainArray filed = { TET_GrassSingle };
	};
	template<>
	struct Terrain<TerrainBlockType::Grass1x2>
	{
		static constexpr TerrainArray field = { TET_GrassLC, TET_GrassRC};
	};
	template<>
	struct Terrain<TerrainBlockType::Grass1x3>
	{
		static constexpr TerrainArray field = { TET_GrassLC, TET_GrassMid, TET_GrassRC};
	};
	template<>
	struct Terrain<TerrainBlockType::Grass1x4>
	{
		static constexpr TerrainArray field = { TET_GrassLC, TET_GrassMid, TET_GrassMid, TET_GrassRC};
	};
	template<>
	struct Terrain<TerrainBlockType::DirtSingle>
	{
		static constexpr TerrainArray field = { TET_DirtSingle };
	};
	template<>
	struct Terrain<TerrainBlockType::Dirt1x2>
	{
		static constexpr TerrainArray field = { TET_DirtLC, TET_DirtRC };
	};
	template<>
	struct Terrain<TerrainBlockType::Dirt1x3>
	{
		static constexpr TerrainArray field = { TET_DirtLC, TET_DirtMid, TET_DirtRC };
	};
	template<>
	struct Terrain<TerrainBlockType::Dirt1x4>
	{
		static constexpr TerrainArray field = { TET_DirtLC, TET_DirtMid, TET_DirtMid, TET_DirtRC };
	};

	struct TerrainData
	{
		TerrainBlockType type = TerrainBlockType::TNone;
		unsigned int top = { 0};
		unsigned int left = { 0};
	};
	template<StageNumber>
	struct StageData
	{
	};

	template<>
	struct StageData<StageNumber::Stage1>
	{
		static constexpr std::array<TerrainData, 2> data = {{
			{TerrainBlockType::Grass1x3, 3, 2},
			{TerrainBlockType::Dirt1x2, 4, 2},
		}};
	};

	class StageTerrain
	{
	private:
		static constexpr unsigned int ROW_LENGHT = 9u;
		static constexpr unsigned int COL_LENGHT = 16u;
		std::array<std::array<TerrainElementType, COL_LENGHT>, ROW_LENGHT> stagePlaced = {};
		std::vector<std::pair<UINT, DirectX::XMFLOAT4X4>> terrainAndWorlds;
		StageNumber stageNumber;
		std::shared_ptr<SceneModel> terrainModels;

	public:
		using StageArray = std::array<std::array<TerrainElementType, COL_LENGHT>, ROW_LENGHT>;
		StageTerrain() = default;

		void Initialize(StageNumber number, const int offset = 8.0f, const int xAxisMax = 60.0f, const int yAxisMax = 32.0f);

		StageArray GetStagePlaced() const { return stagePlaced; }

		void Render(RenderContext& rc, ModelRenderer* mR);
	};
}
