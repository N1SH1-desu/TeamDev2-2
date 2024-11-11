#pragma once
#include<d3d11.h>


class Fogs
{
    Fogs();
    ~Fogs() = default;

    void Update();
    void Render(ID3D11DeviceContext* dc);
    void DrawImgui();
    
};