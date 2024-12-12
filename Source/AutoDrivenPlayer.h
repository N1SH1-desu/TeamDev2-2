#pragma once

#include <memory>

#include "Model.h"
#include "FetchModelFromSceneAsset.h"
#include "ModelRenderer.h"
#include "StageEditer.h"
#include "Tetromino.h"

class AutoDrivenPlayer
{
public:
    AutoDrivenPlayer();

    void Update(float elapsedTime, const Stage::StageTerrain::StageArray stagePlaced, const Tetromino::TetrominoCollider::TetroCollideArray tetroPlaced);

    void Render(RenderContext& rc, ModelRenderer* mR);

private:
    bool MoveForward(float elapsedTime);

    void Falling(float elapsedTime, const Stage::StageTerrain::StageArray stagePlaced, const Tetromino::TetrominoCollider::TetroCollideArray tetroPlaced);

    void UpdateTransform();

private:
    struct TransformElements
    {
        DirectX::XMFLOAT3 position = { 0.0f, -5.0f, 0.0f };
        DirectX::XMFLOAT3 scale = { 0.05f, 0.05f, 0.05f };
        DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
    };
    TransformElements transformElements;
    DirectX::XMFLOAT4X4 worldTransform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

    struct VelocityManegement
    {
        const float acceleration = 50.0f;
        const float moveSpeed = 5.0f;
        const float gravity = 10.0f;
        bool dirIdentify = false;
    };
    const VelocityManegement veloManagement;
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    enum class State
    {
        Run,
        Climb,
    }state;

    bool onGround = false;

    std::unique_ptr<Model>  model;
};
