#pragma once

#include <memory>

#include "Model.h"

class AutoDrivenPlayer
{
public:
    AutoDrivenPlayer();

    void Initialize();

    void Update(float elapsedTime);

private:
    struct TransformElements
    {
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
    };
    TransformElements transformElements;

    struct VelocityManegement
    {
        const float acceleration = 50.0f;
        const float moveSpeed = 5.0f;
        const float gravity = 10.0f;
    };
    VelocityManegement veloManagement;
    float velocityX = 0.0f;

    std::unique_ptr<Model>  model
};
