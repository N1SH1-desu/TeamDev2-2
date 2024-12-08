#include "pause.h"

#include"Graphics.h"
#include"SceneTitle.h"
#include"SceneStageSelect.h"
#include"SceneManager.h"
#include"Scene/AnimationScene.h"

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
    // サンプラーステート設定忘れのエラーコード対策
    {
        HRESULT hr{ S_OK };

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
        hr = id->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());
    }
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
    if (this->flag_)
    {
        if (GetAsyncKeyState('0') & 0x01)
        {
            SceneManager::Instance().ChangeScene(new AnimationScene(stage_nun_));
        }
        if (GetAsyncKeyState('9') & 0x01)
        {
            SceneManager::Instance().ChangeScene(new SceneStageSelect);
        }
        if (GetAsyncKeyState('8') & 0x01)
        {
            SceneManager::Instance().ChangeScene(new SceneTitle);
        }
    }
}

void Pause::Render(float elapsedTime)
{
    
    if (this->flag_)
    {

        ID3D11DeviceContext*dc= Graphics::Instance().GetDeviceContext();
        dc->PSSetSamplers(0, 1, sampler_state.GetAddressOf());




        pause_retry_.get()->Render
            (dc
            , base_pos_x_, base_pos_y_,0,
            base_size_x_, base_size_y_, 
            0.f,
            1.f, 1.f, 1.f, 1.f);
        pause_select_.get()->Render
            (dc,
            base_pos_x_, (base_pos_y_+base_size_y_ + base_size_y_ * 0.5f),0,
            base_size_x_, base_size_y_, 
            0.f,
            1.f, 1.f, 1.f, 1.f);
        pause_title_.get()->Render
            (dc, 
            base_pos_x_, (base_pos_y_+(3.0f*base_size_y_)), 0,
            base_size_x_, base_size_y_,
            0.f,
            1.f, 1.f, 1.f, 1.f);
    }
}
