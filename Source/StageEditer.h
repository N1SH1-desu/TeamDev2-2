#pragma once
#include <array>
#include "FetchModelFromSceneAsset.h"

namespace Stage
{
	enum StageNumber
	{
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
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

		TNone = -1,
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
	template<>
	struct Terrain<TerrainBlockType::TNone>
	{
		static constexpr TerrainArray field = { TET_None };
	};

	struct TerrainData
	{
		TerrainBlockType type = TerrainBlockType::TNone;
		unsigned int top = { 0};
		unsigned int left = { 0};
	};

	using TerrainDataArray = std::array<TerrainData, 20>;

	template<StageNumber>
	struct StageData
	{
	};

	template<>
	struct StageData<StageNumber::Stage1>
	{
		static constexpr TerrainDataArray data = {{
			{TerrainBlockType::Grass1x3, 1, 13},
			{TerrainBlockType::Grass1x4, 2, 0},
			{TerrainBlockType::Dirt1x3, 5, 6},
			{TerrainBlockType::Dirt1x3, 7, 0},
			{TerrainBlockType::Dirt1x2, 7, 14},
		}};
	};
	template<>
	struct StageData<StageNumber::Stage2>
	{
		static constexpr TerrainDataArray data = {{
			{TerrainBlockType::Grass1x4, 2, 5},
			{TerrainBlockType::GrassSingle, 4, 0},
			{TerrainBlockType::Dirt1x2, 5, 0},
			{TerrainBlockType::Grass1x3, 4, 13},
			{TerrainBlockType::Dirt1x3, 6, 5},
			{TerrainBlockType::Dirt1x3, 8, 0},
			{TerrainBlockType::Dirt1x2, 8, 14},
		}};
	};
	template<>
	struct StageData<StageNumber::Stage3>
	{
		static constexpr TerrainDataArray data = {{
			{TerrainBlockType::Grass1x3, 2, 5},
			{TerrainBlockType::Grass1x2, 4, 0},
			{TerrainBlockType::Dirt1x4, 5, 0},
			{TerrainBlockType::GrassSingle, 3, 12},
			{TerrainBlockType::DirtSingle, 4, 12},
			{TerrainBlockType::Dirt1x3, 5, 10},
			{TerrainBlockType::Grass1x3, 7, 6},
			{TerrainBlockType::Grass1x3, 8, 0},
			{TerrainBlockType::Grass1x3, 8, 13},
		}};
	};
	template<>
	struct StageData<StageNumber::Stage4>
	{
		static constexpr TerrainDataArray data = {{
			{TerrainBlockType::Grass1x3, 1, 0},
			{TerrainBlockType::Grass1x2, 1, 10},
			{TerrainBlockType::Grass1x2, 3, 14},
			{TerrainBlockType::Grass1x2, 4, 0},
			{TerrainBlockType::Dirt1x2, 5, 0},
			{TerrainBlockType::Dirt1x2, 6, 0},
			{TerrainBlockType::Grass1x3, 5, 10},
			{TerrainBlockType::DirtSingle, 6, 12},
			{TerrainBlockType::DirtSingle, 7, 12},
			{TerrainBlockType::Grass1x4, 8, 5},
		}};
	};
	template<>
	struct StageData<StageNumber::Stage5>
	{
		static constexpr TerrainDataArray data = { {
			{TerrainBlockType::Grass1x3, 1, 0},
			{TerrainBlockType::DirtSingle, 2, 0},
			{TerrainBlockType::Dirt1x3, 3, 0},
			{TerrainBlockType::Grass1x2, 1, 13},
			{TerrainBlockType::DirtSingle, 2, 14},
			{TerrainBlockType::GrassSingle, 2, 6},
			{TerrainBlockType::Dirt1x2, 3, 6},
			{TerrainBlockType::Grass1x3, 5, 2},
			{TerrainBlockType::DirtSingle, 6, 3},
			{TerrainBlockType::Dirt1x3, 7, 2},
			{TerrainBlockType::Grass1x3, 7, 7},
			{TerrainBlockType::Grass1x3, 5, 12},
			{TerrainBlockType::Dirt1x3, 8, 13},
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

		const SceneModel* GetSceneModel() const { return terrainModels.get(); }

		void Initialize(StageNumber number, const int offset = 8.0f, const int xAxisMax = 60.0f, const int yAxisMax = 32.0f);

		StageArray GetStagePlaced() const { return stagePlaced; }

		void Render(RenderContext& rc, ModelRenderer* mR);
	};
}
