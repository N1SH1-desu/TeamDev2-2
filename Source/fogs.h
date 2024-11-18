#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include<wrl.h>

class Fogs
{
public:

    Fogs();
    ~Fogs() = default;

    void Update(float elapsedTime);
    void Render(float elapsedTime);
    void DrawImgui();
    
private:
    struct fogConstants
    {
        DirectX::XMFLOAT4 fog_color;
        DirectX::XMFLOAT4 fog_range;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer>fog_constant_buffer_;
    DirectX::XMFLOAT4 fog_color_{ 0.2f,0.2f,0.2f,1.0f };
    DirectX::XMFLOAT4 fog_range_{ 0.1f,100.f,0,0 };     //x:near,y:far,zw:null
};