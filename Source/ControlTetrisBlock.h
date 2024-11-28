#pragma once
#include <InputMouse.h>
#include <DirectXMath.h>
#include <Graphics.h>
#include <cmath>

DirectX::XMFLOAT3 SetBlockPosFromMousePos(InputMouse* input, float grid_size, RECT viewPort, DirectX::XMMATRIX& Projection, DirectX::XMMATRIX& View, DirectX::XMMATRIX& world)
{
    POINTS pos = input->GetPosition();

    UINT xGridIndex = pos.x / static_cast<SHORT>(grid_size);
    UINT yGridIndex = pos.y / static_cast<SHORT>(grid_size);
        

    if (ImGui::Begin("Block Property", nullptr))
    {
    }
    ImGui::End();
}
