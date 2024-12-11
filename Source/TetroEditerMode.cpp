#include "TetroEditerMode.h"

void TetroEditerMode::Initialize(ID3D11Device* device, ID2D1DeviceContext* d2dContext)
{
	Graphics2D* gfx2d = Graphics::Instance().GetGfx2D();

	editerUI.Initialize(device);

	gridRenderer = std::make_unique<Grid2DRenderer>(d2dContext, Graphics::Instance().GetBackBuffer());

	tetroBlockModels = std::make_unique<SceneModel>("Data/Model/TetrisBlock/Colors.mdl");
}

void TetroEditerMode::Update(float elapsedTime, POINTS mousePos, const Input::KeyInput& keyInput)
{
	if (keyInput.GetKeyStatus('G') == Input::Release)
	{
		EditerMode = !EditerMode;
	}

	gridRenderer->Update(elapsedTime, EditerMode);

	if (EditerMode)
	{
		tetroEditer.Update(mousePos, keyInput, tetroBlockModels.get());
	}

	static bool hoge = false;
	if (keyInput.GetKeyStatus(VK_TAB) == Input::Release)
	{
		hoge = !hoge;
	}
	editerUI.Update(elapsedTime, hoge);
}

void TetroEditerMode::Render(RenderContext rc, ID2D1DeviceContext* d2dContext, ModelRenderer* mRenderer)
{
	if (EditerMode)
	{
		tetroEditer.Render(rc, mRenderer);
	}
	gridRenderer->Draw(d2dContext);

	editerUI.Render(rc.deviceContext);
}
