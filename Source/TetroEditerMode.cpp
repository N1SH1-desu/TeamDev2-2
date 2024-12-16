#include "TetroEditerMode.h"

void TetroEditerMode::Initialize(ID3D11Device* device, ID2D1DeviceContext* d2dContext)
{
	Graphics2D* gfx2d = Graphics::Instance().GetGraphics2D();

	editerUI.Initialize(device);

	gridRenderer = std::make_unique<Grid2DRenderer>(d2dContext, Graphics::Instance().GetBackBuffer());

	tetroBlockModels = std::make_shared<SceneModel>("Data/Model/TetrisBlock/Colors.mdl");
}

void TetroEditerMode::Update(float elapsedTime, POINTS mousePos, const Input::KeyInput& keyInput, const TerrainStage::StageTerrain::StageArray& stageCollision)
{
	if (keyInput.GetKeyStatus('G') == Input::Release)
	{
		EditerMode = !EditerMode;
	}

	gridRenderer->Update(elapsedTime, EditerMode);

	if (EditerMode)
	{
		tetroEditer.Update(mousePos, keyInput, tetroBlockModels.get(), stageCollision);
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

	tetroBlockModels->RenderCommitedBlocks(rc, mRenderer, ShaderId::Lambert, true);

	gridRenderer->Draw(d2dContext);

	editerUI.Render(rc.deviceContext);
}
