#pragma once
#include "Graphics.h"
#include "KeyInput.h"
#include "Tetromino.h"
#include "Grid2DRenderer.h"

class TetroEditerMode
{
public:
	TetroEditerMode() = default;

	void Initialize(ID3D11Device* device);
	void Update(float elapsedTime, POINTS mousePos, const Input::KeyInput& keyInput);
	void Render(RenderContext rc, ID2D1DeviceContext* d2dContext, ModelRenderer* mRenderer);

private:

};
