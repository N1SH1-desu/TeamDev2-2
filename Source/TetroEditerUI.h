#pragma once
#include "Sprite.h"

#include <memory>
#include <d3d11.h>

class TetroEditerUI
{
public:
    TetroEditerUI() = default;

    void Initialize(ID3D11Device* device);

    void Update(float elapsedTime, bool isOned);

    void Render(ID3D11DeviceContext* dc);

private:
    std::unique_ptr<Sprite> Panel;
    float panelOffset = 0.0f;
    static constexpr float offset = 608.0f;
};