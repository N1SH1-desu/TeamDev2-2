#include "Grid2DRenderer.h"

Grid2DRenderer::Grid2DRenderer(ID2D1DeviceContext* context, IDXGISurface* targetSurface)
	:
	Gfx2DSandbox(context, targetSurface)
{
	context->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);
}

void Grid2DRenderer::Excecute(ID2D1DeviceContext* context)
{
}
