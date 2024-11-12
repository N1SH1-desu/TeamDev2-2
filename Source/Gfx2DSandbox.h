#pragma once
#include <Gfx2D.h>
#include <string>

class Gfx2DSandbox
{
public:
    Gfx2DSandbox(ID2D1DeviceContext* context, IDXGISurface* targetSurface);
    virtual ~Gfx2DSandbox() = default;
    void Draw(ID2D1DeviceContext* context);
     virtual void Excecute(ID2D1DeviceContext* context) = 0;
private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
};
