#pragma once
#include <Gfx2D.h>

class Gfx2DRenderer
{
public:
    Gfx2DRenderer(ID2D1DeviceContext* context, IDXGISurface* surface);

    void Draw(ID2D1DeviceContext* context);

private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
};
