#include"clear.h"

#include"Graphics.h"
#include"SceneManager.h"
#include"SceneStageSelect.h"
#include"SceneTitle.h"

namespace clear_calc
{
    inline const float EaseOutBounce(float x)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (x < 1 / d1)
        {
            return n1 * x * x;
        }
        else if (x < 2 / d1)
        {
            return n1 * (x -= 1.5f / d1) * x + 0.75f;
        }
        else if (x < 2.5f / d1)
        {
            return n1 * (x -= 2.25f / d1) * x + 0.9375f;
        }
        else
        {
            return n1 * (x -= 2.625f / d1) * x + 0.984375f;
        }
    }
}

Clear::Clear()
{
    ID3D11Device* id = Graphics::Instance().GetDevice();

    HRESULT hr{ S_OK };
    // サンプラーステート設定忘れのエラーコード対策
    {

        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.MipLODBias = 0;
        sampler_desc.MaxAnisotropy = 16;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD = 0;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = id->CreateSamplerState(&sampler_desc, sampler_state_.GetAddressOf());
    }

    sprite_title_ = std::make_unique<Sprite>(id, ".//Data//Sprite//back_to_title.png");
    sprite_select_ = std::make_unique<Sprite>(id, ".//Data//Sprite//back_to_select.png");
    sprite_clear_ = std::make_unique<Sprite>(id, ".//Data//Sprite//clear.png");
    sprite_back_ = std::make_unique<Sprite>(id);

    //スクリーン準拠の割合を入れておく。
    base_x_ = Graphics::Instance().GetScreenWidth() * 0.125f;
    base_y_ = Graphics::Instance().GetScreenHeight() / 6.f;
}

void Clear::Update(float elapsedTime, InputMouse* mouse)
{
    static float animation_timer = 0.f;
    if (flag_)
    {
        if (animation_timer <= 1.f)
        {
            clear_scale_ = clear_calc::EaseOutBounce(animation_timer);
            animation_timer+=elapsedTime;
        }
        else
        {
            clear_scale_ = 1.0f;
        }

        POINTS mouse_pos = mouse->GetPosition();
        
        bool select_pos =
            (((base_x_ * 2.f) < mouse_pos.x && mouse_pos.x < ((base_x_ * 2.f) + base_x_ * 4.f))
                && ((base_y_ * 3.f) < mouse_pos.y && mouse_pos.y < (base_y_ * 3.f + base_y_)));
        if (select_pos )
        {
            select_scale_ = 1.2f;
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            {
                SceneManager::Instance().ChangeScene(new SceneStageSelect);
                animation_timer = 0.f;
            }
        }
        else
        {
            select_scale_ = 1.0f;
        }
        bool title_pos =
            (((base_x_ * 2.f) < mouse_pos.x && mouse_pos.x < ((base_x_ * 2.f) + base_x_ * 4.f))
                && ((base_y_ * 4.f) < mouse_pos.y && mouse_pos.y < (base_y_ * 4.f + base_y_)));
        if (title_pos)
        {
            title_scale_ = 1.2f;
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            {
                SceneManager::Instance().ChangeScene(new SceneTitle);
                animation_timer = 0.f;
            }
        }
        else
        {
            title_scale_ = 1.0f;
        }
    }
    else
    {
        animation_timer = 0.f;
    }
}

void Clear::Render(float elapsedTime)
{
    if (flag_)
    {
        ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
        //サンプラーステートの設定
        dc->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());

        sprite_back_.get()->Render(
            dc,
            0, 0, 0,
            base_x_ * 8.f, base_y_ * 6.f,
            0,
            0,0, 0, 0.5f
        );


        sprite_clear_.get()->Render(
            dc,
            (base_x_ * 4.0f) * (1.f-clear_scale_), (base_y_*3.f)*(1.f-clear_scale_), 0,
            (base_x_ * 8.f)*clear_scale_, (base_y_ * 3.f)*clear_scale_,
            0,
            1, 1, 1, 1
        );
        sprite_select_.get()->Render(
            dc,
            base_x_*2.f, base_y_*3.f, 0,
            (base_x_ * 4.f)*select_scale_, base_y_*select_scale_,
            0,
            1, 1, 1, 1
        );
        sprite_title_.get()->Render(
            dc,
            base_x_*2.f, base_y_*4.f, 0,
            (base_x_ * 4.f)*title_scale_, base_y_*title_scale_,
            0,
            1, 1, 1, 1
        );
    }
}
