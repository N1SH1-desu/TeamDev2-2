#include "pause.h"

#include<Windows.h>

#include"Misc.h"
#include"SceneTitle.h"
#include"SceneStageSelect.h"
#include"SceneManager.h"
#include"Scene/AnimationScene.h"

namespace pause_calc
{
    //一秒間掛けてこの動きで返り血を動かす
    inline constexpr float EaseInOutBack(float x)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;

        return (x < 0.5f)
            ? (static_cast<float>(pow(2 * x, 2)) * ((c2 + 1.f) * 2 * x - c2)) / 2.f
            : (static_cast<float>(pow((2 * x) - 2, 2)) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2) / 2.f;
    }
}

Pause::Pause()
    :flag_{false}
    ,animation_timer_{0.f}
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

    //座標とサイズ
    float screen_w = Graphics::Instance().GetScreenWidth();
    float screen_h = Graphics::Instance().GetScreenHeight();
    base_pos_x_ = screen_w * 0.25f;
    base_pos_y_ = screen_h / 6.f;
    base_size_x_ = screen_w * 0.5f;
    base_size_y_ = screen_h / 6.f;


    pause_retry_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\retry.png");
    pause_select_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\back_to_select.png");
    pause_title_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\back_to_title.png");
    pause_back_ = std::make_unique<Sprite>(id);
    pause_key_ = std::make_unique<Sprite>(id,".\\Data\\Sprite\\pause.png");
}

void Pause::SetPause(bool flag)
{
    this->flag_ = flag;
}

void Pause::Update(float elapsedTime,InputMouse* mouse)
{
    
    //いずれかの処理が行われシーンが変わるならtrue;
    bool scene_changed = false;


    if (this->flag_)
    {
        POINTS mouse_pos = mouse->GetPosition();

        bool retry_bool_pos = (
            (base_pos_x_ < mouse_pos.x && mouse_pos.x < (base_pos_x_ + base_size_x_) &&
                ((base_pos_y_ < mouse_pos.y && mouse_pos.y < base_pos_y_ + base_size_y_)))
            );
        bool select_bool_pos = (
            (base_pos_x_ < mouse_pos.x && mouse_pos.x < (base_pos_x_ + base_size_x_) &&
                (
                    ((base_pos_y_ + base_size_y_ * 1.5f) < mouse_pos.y
                        && mouse_pos.y < (base_pos_y_ + base_size_y_ * 1.5f) + base_size_y_)
                    )
                )
            );
        bool title_bool_pos = (
            (base_pos_x_ < mouse_pos.x && mouse_pos.x < (base_pos_x_ + base_size_x_) &&
                (
                    ((base_pos_y_ + base_size_y_ * 3.f) < mouse_pos.y
                        && mouse_pos.y < (base_pos_y_ + base_size_y_ * 3.f) + base_size_y_)
                    )
                )
            );

        if (retry_bool_pos)
        {
            scale_[0] = 1.2f;
            if ((GetAsyncKeyState('0') & 0x8000)
                || (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                )
            {
                SceneManager::Instance().ChangeScene(new AnimationScene(stage_nun_));
                scene_changed = true;
            }
        }
        else
        {
            scale_[0] = 1.f;
        }
        if(select_bool_pos)
        {
            scale_[1] = 1.2f;
            if (GetAsyncKeyState('9') & 0x8000
                || (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                )
            {
                SceneManager::Instance().ChangeScene(new SceneStageSelect);
                scene_changed = true;
            }
        }
        else
        {
            scale_[1] = 1.0f;
        }
        if (title_bool_pos)
        {
            scale_[2] = 1.2f;
            if (GetAsyncKeyState('8') & 0x8000
                || (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                )
            {
                SceneManager::Instance().ChangeScene(new SceneTitle);
                scene_changed = true;
            }

        }
        else
        {
            scale_[2] = 1.0f;
        }
    }
    else 
    {
        GetAsyncKeyState(VK_SPACE);
        for (int i = 0; i < 3; i++)scale_[i] = 1.0f;
    }

    if (scene_changed) {
        animation_timer_ = 0.f;
    }
}

void Pause::Render(float elapsedTime)
{
    ID3D11DeviceContext*dc= Graphics::Instance().GetDeviceContext();
    //サンプラーステートの設定
    dc->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());

    static float add_scale = 0;
    static float alpha = 0;
   
    float pos_scale_x = base_pos_x_ + ((base_size_x_ * 0.5f) * (1.0f - add_scale));
    float pos_scale_y = base_pos_y_ + ((base_size_y_ * 0.5f) * (1.0f - add_scale));
    float size_scale_x = base_size_x_ * add_scale;
    float size_scale_y = base_size_y_ * add_scale;

    
    if (this->flag_)
    {


        //0~1までのスケールの遷移のイージング関数
        if (animation_timer_ <= 1.f)
        {
            add_scale = pause_calc::EaseInOutBack(animation_timer_);
            alpha = pause_calc::EaseInOutBack(animation_timer_) * 0.5f;
            animation_timer_ += elapsedTime;
        }
        else
        {
            add_scale = 1.f;
            alpha = 0.5f;
        }

        if (add_scale < 0)add_scale = 0;

        //背景
        pause_back_.get()->Render
        (dc, 0, 0, 0,
            Graphics::Instance().GetScreenWidth(), Graphics::Instance().GetScreenHeight(),
            0.f,
            0.f, 0.f, 0.f, alpha);

        pause_retry_.get()->Render
            (dc
                , pos_scale_x , (pos_scale_y), 0,
            size_scale_x*scale_[0], size_scale_y*scale_[0],
            0,
            1.f, 1.f, 1.f, 1.f);
        pause_select_.get()->Render
            (dc,
            pos_scale_x, (pos_scale_y) +(base_size_y_ + base_size_y_ * 0.5f),0,
            size_scale_x*scale_[1], size_scale_y*scale_[1], 
            0,
            1.f, 1.f, 1.f, 1.f);
        pause_title_.get()->Render
            (dc, 
            pos_scale_x, (pos_scale_y) +(3.0f*base_size_y_), 0,
            size_scale_x*scale_[2], size_scale_y*scale_[2],
            0,
            1.f, 1.f, 1.f, 1.f);

    }
    else
    {
        //0~1までのスケールの遷移のイージング関数
        if (animation_timer_ >= 0.f)
        {
            add_scale = pause_calc::EaseInOutBack(animation_timer_);
            alpha = pause_calc::EaseInOutBack(animation_timer_) * 0.5f;
            animation_timer_ -= elapsedTime;
        }
        else
        { 
            add_scale = 0.f;
            alpha = 0.f;
        }

        if (add_scale < 0)add_scale = 0;

        if (animation_timer_ > 0)
        {
            //背景
            pause_back_.get()->Render
            (dc, 0, 0, 0,
                Graphics::Instance().GetScreenWidth(), Graphics::Instance().GetScreenHeight(),
                0.f,
                0.f, 0.f, 0.f, alpha);

            pause_retry_.get()->Render
            (dc
                , pos_scale_x, pos_scale_y, 0,
                size_scale_x, size_scale_y,
                0.f,
                1.f, 1.f, 1.f, 1.f);
            pause_select_.get()->Render
            (dc,
                pos_scale_x, pos_scale_y + (base_size_y_ + base_size_y_ * 0.5f), 0,
                size_scale_x, size_scale_y,
                0.f,
                1.f, 1.f, 1.f, 1.f);
            pause_title_.get()->Render
            (dc,
                pos_scale_x, pos_scale_y + (3.0f * base_size_y_), 0,
                size_scale_x, size_scale_y,
                0.f,
                1.f, 1.f, 1.f, 1.f);
        }
        else
        {
                pause_key_.get()->Render(
                dc,
                base_pos_x_ * 3.f, base_pos_y_ * 5.f, 0,
                base_pos_x_, base_pos_y_,
                0,
                1, 1, 1, 1);
            
        }


    }
}
