#include "Gfx2DSandbox.h"
#include <assert.h>

Gfx2DSandbox::Gfx2DSandbox(ID2D1DeviceContext* context, IDXGISurface* targetSurface)
{
    if (context == nullptr)
    {
        assert(false && "context is invalid");
    }
    if (targetSurface == nullptr)
    {
        assert(false && "surface is invalid");
    }
    D2D1_BITMAP_PROPERTIES1 properties;
    properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    properties.dpiX = 0.0f;
    properties.dpiY = 0.0f;
    properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    properties.colorContext = nullptr;
    HRESULT hr = { S_OK };

    hr = context->CreateBitmapFromDxgiSurface(targetSurface, properties, targetBitmap.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        assert(false && "Failed to create bitmap");
    }

    context->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &brush
    );
}

void Gfx2DSandbox::Draw(ID2D1DeviceContext* context)
{
    context->BeginDraw();

    Excecute(context);

    context->EndDraw();
}
