#include "pause.h"

#include"Graphics.h"

namespace pause_calc
{
    inline constexpr float EaseInOutBack(float x)
    {
        //const float c1 = 1.70158f;
        const float c2 = 1.70158f * 1.525f;

        return (x < 0.5f)
            ? (static_cast<float>(pow(2 * x, 2)) * ((c2 + 1.f) * 2 * x - c2)) / 2.f
            : (static_cast<float>(pow(2 * x, 2)) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2) / 2.f;
    }
}

Pause::Pause()
    :flag_{false}
{
    ID3D11Device* id = Graphics::Instance().GetDevice();

    pause_retry_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\retry.png");
    pause_select_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\back_to_select.png");
    pause_title_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\back_to_title.png");
}

void Pause::SetPause(bool flag)
{
    this->flag_ = flag;
}

void Pause::Update(float elapsedTime)
{
    if (flag_)
    {

    }
}

void Pause::Render(float elapsedTime)
{
    if (flag_)
    {
        ID3D11DeviceContext*dc= Graphics::Instance().GetDeviceContext();


        constexpr float base_pos_x = 320.f;
        constexpr float base_pos_y = 320.f;
        constexpr float base_size_x = 640.f;
        constexpr float base_size_y = 120.f;
        constexpr float base_space = base_size_y * 0.5f;

        pause_retry_.get()->Render
            (dc
            , base_pos_x, base_pos_y,-1,
            base_size_x, base_size_y, 
            0.f,
            1.f, 1.f, 1.f, 1.f);
        pause_select_.get()->Render
            (dc,
            base_pos_x, (base_pos_y+base_space+base_size_y),-1,
            base_size_x, base_size_y, 
            0.f,
            1.f, 1.f, 1.f, 1.f);
        pause_title_.get()->Render
            (dc, 
            base_pos_x, (base_pos_y+(3.0f*base_size_y)), -1,
            base_size_x, base_size_y,
            0.f,
            1.f, 1.f, 1.f, 1.f);
    }
}
