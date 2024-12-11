#include "StageEditer.h"

void Stage::StageTerrain::Initialize(StageNumber number, const int offset, const int xAxisMax, const int yAxisMax)
{
    terrainModels = std::make_shared<SceneModel>("Data/Model/Stage/Terrains.mdl");

    auto GetStageData = [](StageNumber number) ->const auto
        {
            switch (number)
            {
            case Stage::Stage1:
                return StageData<Stage1>::data;
                break;
            case Stage::Stage2:
                return StageData<Stage2>::data;
                break;
            case Stage::Stage3:
                return StageData<Stage3>::data;
                break;
            case Stage::Stage4:
                return StageData<Stage4>::data;
                break;
            case Stage5:
                return StageData<Stage5>::data;
            default:
                break;
            }
        };

    auto stageData = GetStageData(number);

    for (auto& dataEle : stageData)
    {
        auto GetTerrainArray = [](TerrainBlockType type) -> TerrainArray
            {
                switch (type)
                {
                case Stage::TerrainBlockType::GrassSingle:
                    return Terrain<TerrainBlockType::GrassSingle>::filed;
                    break;
                case Stage::TerrainBlockType::Grass1x2:
                    return Terrain<TerrainBlockType::Grass1x2>::field;
                    break;
                case Stage::TerrainBlockType::Grass1x3:
                    return Terrain<TerrainBlockType::Grass1x3>::field;
                    break;
                case Stage::TerrainBlockType::Grass1x4:
                    return Terrain<TerrainBlockType::Grass1x4>::field;
                    break;

                case Stage::TerrainBlockType::DirtSingle:
                    return Terrain<TerrainBlockType::DirtSingle>::field;
                    break;
                case Stage::TerrainBlockType::Dirt1x2:
                    return Terrain<TerrainBlockType::Dirt1x2>::field;
                    break;
                case Stage::TerrainBlockType::Dirt1x3:
                    return Terrain<TerrainBlockType::Dirt1x3>::field;
                    break;
                case Stage::TerrainBlockType::Dirt1x4:
                    return Terrain<TerrainBlockType::Dirt1x4>::field;
                    break;

                case Stage::TerrainBlockType::TNone:
                    return Terrain<TerrainBlockType::TNone>::field;
                    break;
                }
        };

        TerrainArray tArray = GetTerrainArray(dataEle.type);

        for (int col = 0; col < tArray.size(); col++)
        {
            if (tArray[col] > 0)
            {
                if (dataEle.top < ROW_LENGHT && (dataEle.left + col) < COL_LENGHT)
                {
                    stagePlaced[dataEle.top][dataEle.left + col] = tArray[col];
                }
            }
        }
    }

    for (int row = 0; row < stagePlaced.size(); row++)
    {
        for (int col = 0; col < stagePlaced[row].size(); col++)
        {
            if (stagePlaced[row][col] != TerrainElementType::TET_None)
            {
                float x = -xAxisMax + col * offset;
                if (x >= xAxisMax) x = xAxisMax;

                float y = yAxisMax - row * offset;
                if (y <= -yAxisMax) y = -yAxisMax;

                DirectX::XMFLOAT4X4 worldTransform{};
                DirectX::XMMATRIX S = DirectX::XMMatrixScaling(8.5f, 8.5f, 8.5f);
                DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(90.0f), DirectX::XMConvertToRadians(180.0f), 0.0f);
                DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(x, y, 0.0f);
                DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);

                UINT index = static_cast<UINT>(stagePlaced[row][col]);
                std::pair<UINT, DirectX::XMFLOAT4X4>&& pair = { index - 1, worldTransform };

                terrainAndWorlds.emplace_back(pair);
                terrainModels->CommitBlock({ index - 1, worldTransform });
            }
        }
    }
}

void Stage::StageTerrain::Render(RenderContext& rc, ModelRenderer* mR)
{
    terrainModels->RenderCommitedBlocks(rc, mR, ShaderId::Lambert, true);
}

