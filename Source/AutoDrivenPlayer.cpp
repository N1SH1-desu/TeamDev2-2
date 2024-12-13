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

void AutoDrivenPlayer::Update(float elapsedTime, const SceneModel* stageSM, const SceneModel* tetroSM)
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

    Falling(elapsedTime, stageSM, tetroSM);

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

void AutoDrivenPlayer::Falling(float elapsedTime, const SceneModel* stageSM, const SceneModel* tetroSM)
{
    velocityY -= veloManagement.gravity * elapsedTime;

    float moveY = velocityY * elapsedTime;

    DirectX::XMFLOAT3 start = {
        transformElements.position.x,
        transformElements.position.y + 0.5f,
        transformElements.position.z
    };

    DirectX::XMFLOAT3 end = {
        transformElements.position.x,
        transformElements.position.y - moveY,
        transformElements.position.z
    };

    for (auto& stageElement : stageSM->GetCommited())
    {
        const Model* model = stageSM->GetSceneModels()[stageElement.first].get();
        DirectX::XMFLOAT4X4 transform = stageElement.second;
        
        DirectX::XMFLOAT3 p, n;
        if (Collision::RayCast(start, end, transform, model, p, n))
        {
            transformElements.position.y = p.y;
            velocityY = 0.0f;
            moveY = 0.0f;
        }
    }
    for (auto& tetroElement : tetroSM->GetCommited())
    {
        const Model* model = tetroSM->GetSceneModels()[tetroElement.first].get();
        DirectX::XMFLOAT4X4 transform = tetroElement.second;

        DirectX::XMFLOAT3 p, n;
        if (Collision::RayCast(start, end, transform, model, p, n))
        {
            transformElements.position.y = p.y;
            velocityY = 0.0f;
            moveY = 0.0f;
        }
    }

    transformElements.position.y += moveY;
}

void AutoDrivenPlayer::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(transformElements.scale.x, transformElements.scale.y, transformElements.scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(transformElements.angle.x, transformElements.angle.y, transformElements.angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(transformElements.position.x, transformElements.position.y, transformElements.position.z);

    DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);
}
