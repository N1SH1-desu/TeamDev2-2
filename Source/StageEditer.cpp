#include "StageEditer.h"

void Stage::StageTerrainCollision::Initialize(StageNumber number)
{
    auto GetStageData = [](StageNumber number) ->const auto
        {
            switch (number)
            {
            case Stage::Stage1:
                return StageData<Stage1>::data;
                break;
            case Stage::Stage2:
                break;
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
                case Stage::TerrainBlockType::T1x1:
                    return Terrain<TerrainBlockType::T1x1>::filed;
                    break;
                case Stage::TerrainBlockType::T1x2:
                    break;
                case Stage::TerrainBlockType::T1x3:
                    break;
                case Stage::TerrainBlockType::T1x4:
                    break;
                case Stage::TerrainBlockType::T2x1:
                    break;
                case Stage::TerrainBlockType::T2x2:
                    break;
                case Stage::TerrainBlockType::T2x3:
                    break;
                case Stage::TerrainBlockType::T2x4:
                    break;
                case Stage::TerrainBlockType::TNone:
                    break;
                default:
                    break;
                }
        };

        TerrainArray tArray = GetTerrainArray(dataEle.type);

        for (int row = 0; row < tArray.size(); row++)
        {
            for (int col = 0; col < tArray[row].size(); col++)
            {
                if (tArray[row][col] != 0)
                {
                    if (row + dataEle.topleft[0] < ROW_LENGHT && col + dataEle.topleft[1] < COL_LENGHT)
                    {
                        stagePlaced[row + dataEle.topleft[0]][col + dataEle.topleft[1]] = 1;
                    }
                }
            }
        }
    }
}

auto Stage::StageTerrainCollision::GetStageData()
{
}
