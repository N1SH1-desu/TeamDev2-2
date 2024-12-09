#include"clear.h"


#include"Graphics.h"

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
}

void Clear::Update(float elapsedTime, InputMouse* mouse)
{
    if (flag_)
    {

    }
}

void Clear::Render(float elapsedTime)
{
    if (flag_)
    {

    }
}
