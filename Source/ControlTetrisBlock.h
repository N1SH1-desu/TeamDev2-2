#pragma once
#include <InputMouse.h>
#include <DirectXMath.h>
#include <Graphics.h>

DirectX::XMFLOAT3 SetBlockPosFromMousePos(InputMouse* input, float grid_size, RECT viewPort, DirectX::XMMATRIX& Projection, DirectX::XMMATRIX& View, DirectX::XMMATRIX& world)
{
    POINTS pos = input->GetPosition();

    short x = pos.x / (static_cast<short>(grid_size) + 1);
    short y = pos.y / (static_cast<short>(grid_size) + 1);

    x = (static_cast<short>(grid_size) / 2) + (static_cast<short>(grid_size) * x);
    y = (static_cast<short>(grid_size) / 2) + (static_cast<short>(grid_size) * y);

    DirectX::XMVECTOR screenPos = DirectX::XMVectorSet(x, y, 0.0f, 0.0f);

    DirectX::XMFLOAT3 objectSpacePos;
    DirectX::XMStoreFloat3(&objectSpacePos, DirectX::XMVector3Unproject(
        screenPos,
        static_cast<float>(viewPort.left),
        static_cast<float>(viewPort.top),
        static_cast<float>(viewPort.right - viewPort.left),
        static_cast<float>(viewPort.bottom - viewPort.top),
        0.0f,
        1.0f,
        Projection,
        View,
        world
    ));
    objectSpacePos.z = 0.0f;
    return objectSpacePos;
}
