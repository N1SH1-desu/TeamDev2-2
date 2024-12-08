#include "TetroEditerUI.h"

void TetroEditerUI::Initialize(ID3D11Device* device)
{
    Panel = std::make_unique<Sprite>(device, "Data/Sprite/Big.png");
}

void TetroEditerUI::Update(float elapsedTime, bool isOned)
{
    static float timer = 0.0f;

    if (isOned)
    {
        timer += elapsedTime * 2;
        if (timer > 1.0f)
            timer = 1.0f;
    }
    else
    {
        timer -= elapsedTime * 2;
        if (timer < 0.0f)
            timer = 0.0f;
    }

    auto easing = [](float timer) -> float
        {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;

            return 1 + c3 * powf(timer - 1.0f, 3) + c1 * powf(timer - 1, 2);
        };

    float hoge = easing(timer);

    auto lerp = [](float v1, float v2, float s) -> float
        {
            return (1.0f - s) * v1 + s * v2;
        };

    panelOffset = lerp(0.0f, offset, hoge);


}

void TetroEditerUI::Render(ID3D11DeviceContext* dc)
{
    Panel->Render(dc, -offset + panelOffset, 0.0f, 0.0f, 608, 408, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}
