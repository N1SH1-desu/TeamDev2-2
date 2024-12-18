#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <mutex>
#include "RenderState.h"
#include "PrimitiveRenderer.h"
#include "ShapeRenderer.h"
#include "ModelRenderer.h"
#include "EndlessGridRenderer.h"
#include "Gfx2D.h"
#include "Grid2DRenderer.h"

// グラフィックス
class Graphics
{
private:
	Graphics() = default;
	~Graphics() = default;

public:
	// インスタンス取得
	static Graphics& Instance()
	{
		static Graphics instance;
		return instance;
	}

	// 初期化
	void Initialize(HWND hWnd);

	// クリア
	void Clear(float r, float g, float b, float a);

	// レンダーターゲット設定
	void SetRenderTargets();

	// 画面表示
	void Present(UINT syncInterval);

	// ウインドウハンドル取得
	HWND GetWindowHandle() { return hWnd; }

	// デバイス取得
	ID3D11Device* GetDevice() { return device.Get(); }

	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() { return immediateContext.Get(); }

	// スクリーン幅取得
	float GetScreenWidth() const { return screenWidth; }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screenHeight; }

	// レンダーステート取得
	RenderState* GetRenderState() { return renderState.get(); }

	// プリミティブレンダラ取得
	PrimitiveRenderer* GetPrimitiveRenderer() const { return primitiveRenderer.get(); }

	// シェイプレンダラ取得
	ShapeRenderer* GetShapeRenderer() const { return shapeRenderer.get(); }

	// モデルレンダラ取得
	ModelRenderer* GetModelRenderer() const { return modelRenderer.get(); }

	EndlessGridRenderer* GetEndlessGridRenderer() const { return endlessGridRenderer.get(); }

	Graphics2D* GetGraphics2D() const { return graphics2D.get(); }
	IDXGISurface* GetBackBuffer() {
		Microsoft::WRL::ComPtr<IDXGISurface> backBuf;
		swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuf));
		return backBuf.Get();
	}

	//ミューテックス取得
	std::mutex& GetMutex() { return mutex; } //排他制御

private:
	HWND											hWnd = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	D3D11_VIEWPORT									viewport;

	float	screenWidth = 0;
	float	screenHeight = 0;

	std::unique_ptr<RenderState>					renderState;
	std::unique_ptr<PrimitiveRenderer>				primitiveRenderer;
	std::unique_ptr<ShapeRenderer>					shapeRenderer;
	std::unique_ptr<ModelRenderer>					modelRenderer;
	std::unique_ptr<EndlessGridRenderer>			endlessGridRenderer;

	std::unique_ptr<Graphics2D>						graphics2D;
	std::mutex mutex;
};
