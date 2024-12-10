#pragma once
#include <InputMouse.h>
#include <DirectXMath.h>
#include <Graphics.h>
#include <cmath>

DirectX::XMFLOAT3 SetBlockPosFromMousePos(float grid_size, RECT viewPort, DirectX::XMMATRIX& Projection, DirectX::XMMATRIX& View, DirectX::XMMATRIX& world)
{
    POINTS pos = InputMouse::Instance().GetPosition();

    short x = pos.x / (static_cast<short>(grid_size));
    short y = pos.y / (static_cast<short>(grid_size));

   /* x = (static_cast<short>(grid_size) / 2) + (static_cast<short>(grid_size) * x);
    y = (static_cast<short>(grid_size) / 2) + (static_cast<short>(grid_size) * y);*/

    DirectX::XMVECTOR screenPos = DirectX::XMVectorSet(x, y, 0.0f, 0.0f);

    DirectX::XMFLOAT3 objectSpacePos{};
    //DirectX::XMStoreFloat3(&objectSpacePos, DirectX::XMVector3Unproject(
    //    screenPos,
    //    static_cast<float>(viewPort.left), static_cast<float>(viewPort.top),
    //    static_cast<float>(viewPort.right - viewPort.left), static_cast<float>(viewPort.bottom - viewPort.top),
    //    0.0f, 1.0f,
    //    Projection,
    //    View,
    //    world
    //));
    //objectSpacePos.z = 0.0f;

    {
        float fromMin = 0;
        float fromMax = 16;
        float toMin = -14;
        float toMax = 16;

        int value = static_cast<int>(toMin + ((x - fromMin) / (fromMax - fromMin)) * (toMax - toMin));
        if (value % 2 != 0)
        {
            value++;
        }
        objectSpacePos.x = value;
    }

    {
        float fromMin = 0;
        float fromMax = 9;
        float toMin = 9;
        float toMax = -7;

        float value = toMin + (((y - fromMin) / (fromMax - fromMin)) * (toMax - toMin));
        //value = std::round(value);
        objectSpacePos.y = value;
    }

   /* if (ImGui::Begin("Block Property", nullptr))
    {
    }
    ImGui::End();*/
    
    return objectSpacePos;
}
