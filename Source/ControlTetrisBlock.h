#pragma once
#include <InputMouse.h>
#include <DirectXMath.h>
#include <Graphics.h>
#include <cmath>

DirectX::XMFLOAT3 SetBlockPosFromMousePos(InputMouse* input, float grid_size)
{
    POINTS pos = input->GetPosition();

    UINT xGridIndex = pos.x / static_cast<SHORT>(grid_size);
    UINT yGridIndex = pos.y / static_cast<SHORT>(grid_size);

    constexpr float xAxisMax = 60.0f;
    constexpr float yAxisMax = 32.0f;

    float x = -xAxisMax + (xGridIndex * 8.0f);
    if (x >= xAxisMax) x = xAxisMax;

    float y = yAxisMax - (yGridIndex * 8.0f);
    if (y <= -yAxisMax) y = -yAxisMax;

    return { x, y, 0.0f };
}
