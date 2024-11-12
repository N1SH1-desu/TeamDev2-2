#pragma once

#include <Gfx2DSandbox.h>

class Grid2DRenderer : public Gfx2DSandbox
{
public:
	Grid2DRenderer(ID2D1DeviceContext* context, IDXGISurface* targetSurface);
	virtual void Excecute(ID2D1DeviceContext* context) override;

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
};
