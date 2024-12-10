#include"number_manager.h"


#include"Graphics.h"

//指定された数字から画像の切り出し位置と切り出しサイズを返す
//x:切り出し位置のx座標
//y:切り出し位置のy座標
//z:切り出しサイズのⅩ
//w:切り出しサイズのy
inline DirectX::XMFLOAT4 CutNumber(int number)
{
    const int number_size_x = 142;
    const int number_size_y = 210;

    DirectX::XMFLOAT4 cut_number;

    cut_number.x = number_size_x * static_cast<float>(number);
    cut_number.y = 0;
    cut_number.z = number_size_x;
    cut_number.w = number_size_y;

    return cut_number;
}

NumberManager::NumberManager()
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
    
    sprite_number_ = std::make_unique<Sprite>(id, ".\\Data\\Sprite\\number.png");
}

void NumberManager::SetTimer(int timer)
{
    //タイマーを取得
    this->timer_ = static_cast<int>(timer);
    //高精度タイマーの周波数を取得
    QueryPerformanceFrequency(&frequency_);
    //計測開始時刻を取得
    QueryPerformanceCounter(&start_);
}

void NumberManager::UpdateTimer()
{
    //現在の時間を取得
    QueryPerformanceCounter(&now_);

    //経過時刻を秒単位で計算
    float elapsedTime = static_cast<float>(now_.QuadPart - start_.QuadPart) / frequency_.QuadPart;

    if (elapsedTime >= 1.0f)
    {
        this->timer_ -= 1.f;
        QueryPerformanceCounter(&start_);
    }
    //負荷軽減(CPUの制限)
    //Sleep(1);
}

void NumberManager::DrawTimer(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
    dc->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());

    int tenth_minute = (static_cast<int>(timer_) / 600 > 0) ? static_cast<int>(timer_) / 600 : 0;
    int first_minute = ((static_cast<int>(timer_) % 600) / 60 > 0) ? (static_cast<int>(timer_) % 600) / 60 : 0;
    int tenth_second = ((static_cast<int>(timer_) % 60) / 10 > 0) ? (static_cast<int>(timer_) % 60) / 10 : 0;
    int first_second = ((static_cast<int>(timer_) % 10) > 0) ? static_cast<int>(timer_) % 10 : 0;    

    float number_size_x;
    number_size_x = size.x / 4.f;

    DirectX::XMFLOAT4 cut_position{};
    cut_position = CutNumber(tenth_minute);
    sprite_number_.get()->Render(
        dc,
        pos.x+number_size_x*0, pos.y, sprite_depth_,
        number_size_x, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);
    cut_position = CutNumber(first_minute);
    sprite_number_.get()->Render(
        dc,
        pos.x+number_size_x*1, pos.y, sprite_depth_, 
        number_size_x, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);

    //:を表示するためのスプライト
    sprite_number_.get()->Render(
        dc,
        pos.x + number_size_x * 2, pos.y, sprite_depth_,
        size.x * 0.1f, size.y,
        1420, 0, 60, 210,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);

    cut_position = CutNumber(tenth_second);
    sprite_number_.get()->Render(
        dc,
        pos.x+number_size_x*2+size.x*0.1f, pos.y, sprite_depth_,
        number_size_x, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);
    cut_position = CutNumber(first_second);
    sprite_number_.get()->Render(
        dc,
        pos.x+number_size_x*3+size.x*0.1f, pos.y, sprite_depth_,
        number_size_x, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);
}

void NumberManager::DrawNumber(int number, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
    dc->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());

    const int tenth =((number)%100)/10;
    const int first =((number)%10);

    DirectX::XMFLOAT4 cut_position = CutNumber(tenth);
    sprite_number_.get()->Render(
        dc,
        pos.x, pos.y, sprite_depth_,
        size.x*0.5f, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);

    cut_position = CutNumber(first);
    sprite_number_.get()->Render(
        dc,
        pos.x+size.x*0.5f, pos.y, sprite_depth_,
        size.x*0.5f, size.y,
        cut_position.x, cut_position.y, cut_position.z, cut_position.w,
        DirectX::XMConvertToRadians(0),
        1.f, 1.f, 1.f, 1.f);
}
