#include "Grid2DRenderer.h"
#include "Graphics.h"

#include <math.h>

Grid2DRenderer::Grid2DRenderer(ID2D1DeviceContext* context, IDXGISurface* targetSurface)
	:
	Gfx2DSandbox(context, targetSurface)
{
	context->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black, 0.0f),
		&brush
	);
}

void Grid2DRenderer::Update(float elapsedTime, bool check)
{
	auto pls = [](float t) -> float
		{
			return 1 - powf(1 - t, 3);
		};

	static float timer = 0.0f;

	if (check)
	{
		timer += elapsedTime;
	}
	else
	{
		timer -= elapsedTime;
	}
	if (timer > 1.0f) { timer = 1.0f; }
	if (timer < 0.0f) { timer = 0.0f; }

	float x = pls(timer);

	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black, x));
}

void Grid2DRenderer::Excecute(ID2D1DeviceContext* context)
{
	float screen_width = Graphics::Instance().GetScreenWidth();
	float screen_height = Graphics::Instance().GetScreenHeight();

	for (int x = 0; x < screen_width; x += grid_size)
	{
		context->DrawLine(
			D2D1::Point2F(static_cast<float>(x), 0.0f),
			D2D1::Point2F(static_cast<float>(x), screen_height),
			brush.Get(),
			1.0f
		);
	}

	for (int y = 0; y < screen_height; y += grid_size)
	{
		context->DrawLine(
			D2D1::Point2F(0.0f, static_cast<float>(y)),
			D2D1::Point2F(screen_width, static_cast<float>(y)),
			brush.Get(),
			1.0f
		);
	}
}
