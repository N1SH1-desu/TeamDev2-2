#include "Gfx2DRenderer.h"
#include <assert.h>

Gfx2DRenderer::Gfx2DRenderer(ID2D1DeviceContext* context, IDXGISurface* surface)
{
    if (context == nullptr)
    {
        assert(false && "context is invalid");
    }
    if (surface == nullptr)
    {
        assert(false && "surface is invalid");
    }
    D2D1_BITMAP_PROPERTIES1 properties;
    properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
    properties.dpiX = 0.0f;
    properties.dpiY = 0.0f;
    properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
    properties.colorContext = nullptr;
    HRESULT hr = { S_OK };

    hr = context->CreateBitmapFromDxgiSurface(surface, properties, bitmap.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        assert(false && "Failed to create bitmap");
    }
}

void Gfx2DRenderer::Draw(ID2D1DeviceContext* context)
{
}
