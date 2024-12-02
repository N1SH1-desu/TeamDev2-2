#pragma once
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <d2d1.h>
#include <d2d1_1helper.h>
#include <d3d11.h>
#include <wrl.h>

class Graphics2D
{
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    Graphics2D(ID3D11Device* device);

    ID2D1DeviceContext* GetContext() const { return d2dContext.Get(); }
private:
    ComPtr<ID2D1Factory1> d2dFactory;
    ComPtr<ID2D1Device> d2dDevice;
    ComPtr<ID2D1DeviceContext> d2dContext;

    static bool instanced;
};
