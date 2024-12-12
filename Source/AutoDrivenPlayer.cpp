#include "AutoDrivenPlayer.h"
#include "Collision.h"

AutoDrivenPlayer::AutoDrivenPlayer()
    :
    transformElements(),
    state(State::Run),
    model()
{
    model = std::make_unique<Model>("Data/Model/UnityChan/UnityChan.mdl");
}

void AutoDrivenPlayer::Update(float elapsedTime, const TerrainStage::StageTerrain::StageArray stagePlaced, const Tetromino::TetrominoCollider::TetroCollideArray tetroPlaced)
{
    switch (state)
    {
    case AutoDrivenPlayer::State::Run:
        //if (MoveForward(elapsedTime))
        //{
        //    state = State::Climb;
        //}
        break;
    case AutoDrivenPlayer::State::Climb:
        break;
    }

    Falling(elapsedTime, stagePlaced, tetroPlaced);

    UpdateTransform();
}

void AutoDrivenPlayer::Render(RenderContext& rc, ModelRenderer* mR)
{
    mR->Render(rc, worldTransform, model.get(), ShaderId::Lambert, true);
}

bool AutoDrivenPlayer::MoveForward(float elapsedTime)
{
    float acceleration = (veloManagement.dirIdentify ? -1.0f : 1.0f * veloManagement.acceleration) * elapsedTime;
    velocityX += acceleration;

    float velocityLength = sqrtf(velocityX * velocityX);
    if (velocityLength > veloManagement.moveSpeed)
    {
        velocityX = veloManagement.moveSpeed;
    }

    DirectX::XMFLOAT3 start = {
        transformElements.position.x,
        transformElements.position.y + 0.5f,
        transformElements.position.z
    };

    DirectX::XMFLOAT3 end = {
        transformElements.position.x + velocityX,
        transformElements.position.y + 0.5f,
        transformElements.position.z
    };

    DirectX::XMFLOAT3 p, n;

    return false;
}

void AutoDrivenPlayer::Falling(float elapsedTime, const TerrainStage::StageTerrain::StageArray stagePlaced, const Tetromino::TetrominoCollider::TetroCollideArray tetroPlaced)
{
    velocityY -= veloManagement.gravity * elapsedTime;
    float moveY = velocityY * elapsedTime;

    constexpr float yAxisMax = 32.0f;
    constexpr float xAxisMax = 60.0f;
    constexpr float offset = 8.0f;

    int rowCur = static_cast<int>((transformElements.position.y - yAxisMax) / -offset);
    int rowNext = static_cast<int>(((transformElements.position.y + moveY) - yAxisMax) / -offset);

    int colCur = static_cast<int>((transformElements.position.x - xAxisMax) / -offset);



}

void AutoDrivenPlayer::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(transformElements.scale.x, transformElements.scale.y, transformElements.scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(transformElements.angle.x, transformElements.angle.y, transformElements.angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(transformElements.position.x, transformElements.position.y, transformElements.position.z);

    DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);
}
