#include "Gfx2D.h"
#include <assert.h>

bool Graphics2D::instanced = false;

Graphics2D::Graphics2D(ID3D11Device* device) :
    d2dFactory(),
    d2dDevice(),
    d2dContext()
{
    if (instanced)
    { 
        assert(false && "Graphics2D is instanced");
    }
    if (device == nullptr)
    {
        assert(false && "Invalid D3DDevice");
    }

    D2D1_FACTORY_OPTIONS options;
    options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#ifdef _DEBUG
    options.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
#endif // _DEBUG

    HRESULT hr = { S_OK };

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void**>(d2dFactory.ReleaseAndGetAddressOf()));
    if (FAILED(hr))
    {
        assert(false && "Failed to create Factory");
    }

    ComPtr<IDXGIDevice> dxgiDevice;
    hr = device->QueryInterface<IDXGIDevice>(&dxgiDevice);
    if (FAILED(hr))
    {
        assert(false && "Failed to query dxgiDevice");
    }
    if (dxgiDevice == nullptr)
    {
        assert(false && "dxgiDevice is nullptr");
    }

    hr = d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);
    if (FAILED(hr))
    {
        assert(false && "Failed to create d2dDevice");
    }

    hr = d2dDevice->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        &d2dContext
    );
    if (FAILED(hr))
    {
        assert(false && "Failed to create d2dContext");
    }

    instanced = true;
}
