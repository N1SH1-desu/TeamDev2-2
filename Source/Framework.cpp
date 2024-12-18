#include <memory>
#include <sstream>
#include <imgui.h>
#include <windowsx.h>

#include "Framework.h"
#include "Graphics.h"
#include "ImGuiRenderer.h"

#include"Scene/RayCastScene.h"
#include "Scene/AnimationScene.h"

#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "InputMouse.h"

#define MANAGER 1

// 垂直同期間隔設定
static const int syncInterval = 1;

// コンストラクタ
Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
{
	// グラフィックス初期化
	Graphics::Instance().Initialize(hWnd);
	InputMouse::Instance().Initialize(hWnd);


	// IMGUI初期化
	ImGuiRenderer::Initialize(hWnd, Graphics::Instance().GetDevice(), Graphics::Instance().GetDeviceContext());

	// シーン初期化
	//scene = std::make_unique<RayCastScene>();
	//scene = std::make_unique<LandWalkScene>();
	//scene = std::make_unique<SlideMoveScene>();
	//scene = std::make_unique<MoveFloorScene>();
	//scene = std::make_unique<TerrainAlignScene>();
	//scene = std::make_unique<AnimationScene>();
	//scene = std::make_unique<Player>();
	//scene = std::make_unique<AttachWeaponScene>();
	//scene = std::make_unique<ProjectScreenScene>();
	//scene = std::make_unique<ResourceManagementScene>();
	//scene = std::make_unique<UIAnimScene>();
	//scene = std::make_unique<HitStopScene>();

#if MANAGER
	SceneManager::Instance().ChangeScene(new SceneTitle);
#else
	scene = std::make_unique<RayCastScene>();
#endif
}

// デストラクタ
Framework::~Framework()
{
	// IMGUI終了化
	ImGuiRenderer::Finalize();

	SceneManager::Instance().Clear();

	PlayerManager::Instance().AllDelete();
}

// 更新処理
void Framework::Update(float elapsedTime)
{
	// IMGUIフレーム開始処理	
	ImGuiRenderer::NewFrame();

	InputMouse::Instance().ProcessCommand();

#if MANAGER
	SceneManager::Instance().Update(elapsedTime);
#else
	scene->SetInputMouse(&mouse);
	scene->Update(elapsedTime);
#endif
}

// 描画処理
void Framework::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	// 画面クリア
	Graphics::Instance().Clear(0.5f, 0.5f, 0.5f, 1);

	// レンダーターゲット設定
	Graphics::Instance().SetRenderTargets();

	// シーン描画処理

	// シーンGUI描画処理

#if MANAGER
	SceneManager::Instance().Render(elapsedTime);
#else
	scene->Render(elapsedTime);
	scene->DrawGUI();
#endif

	// シーン切り替えGUI
	//SceneSelectGUI();
#if 0
	// IMGUIデモウインドウ描画（IMGUI機能テスト用）
	ImGui::ShowDemoWindow();
#endif
	// IMGUI描画
	ImGuiRenderer::Render(dc);

	// 画面表示
	Graphics::Instance().Present(syncInterval);
}

template<class T>
void Framework::ChangeSceneButtonGUI(const char* name)
{
	if (ImGui::Button(name))
	{
		if (scene)
		{
			scene.reset();
		}
		scene = std::make_unique<T>();
	}
}

// シーン切り替えGUI
void Framework::SceneSelectGUI()
{
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	float width = 210;
	float height = 460;
	ImGui::SetNextWindowPos(ImVec2(pos.x + displaySize.x - width - 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Once);

	//if (ImGui::Begin("Scene"))
	//{
	//	//ChangeSceneButtonGUI<RayCastScene>(u8"01.レイキャスト");
	//	ChangeSceneButtonGUI<LandWalkScene>(u8"02.地上歩行");
	//	ChangeSceneButtonGUI<SlideMoveScene>(u8"03.壁ずり移動");
	//	ChangeSceneButtonGUI<AnimationScene>(u8"04.アニメーション");
	//	ChangeSceneButtonGUI<ProjectScreenScene>(u8"05.スクリーン座標変換");
	//	ChangeSceneButtonGUI<AttachWeaponScene>(u8"06.アタッチメント");
	//	ChangeSceneButtonGUI<HitStopScene>(u8"07.ヒットストップ");
	//	ChangeSceneButtonGUI<UIAnimScene>(u8"08.UI演出");
	//	ChangeSceneButtonGUI<MoveFloorScene>(u8"09.移動床");
	//	ChangeSceneButtonGUI<TerrainAlignScene>(u8"10.地形に沿う姿勢制御");
	//	ChangeSceneButtonGUI<ResourceManagementScene>(u8"11.リソース管理");
	//}
	//ImGui::End();
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

// アプリケーションループ
int Framework::Run()
{
	MSG msg = {};

	HDC hDC = GetDC(hWnd);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();

			float elapsedTime = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / static_cast<float>(GetDeviceCaps(hDC, VREFRESH))
				;
			
			Update(elapsedTime);
			Render(elapsedTime);
		}
	}

	ReleaseDC(hWnd, hDC);

	return static_cast<int>(msg.wParam);
}

// メッセージハンドラ
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	InputMouse& mouse = InputMouse::Instance();
	if (ImGuiRenderer::HandleMessage(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	case WM_MOUSEMOVE:
		mouse.InQueueCommand({ MouseCommand::M_MOVE, MAKEPOINTS(lParam) });
		break;
	case WM_LBUTTONDOWN:
		mouse.InQueueCommand({ MouseCommand::M_LBUTTON_DOWN });
		break;
	case WM_RBUTTONDOWN:
		mouse.InQueueCommand({ MouseCommand::M_RBUTTON_DOWN });
		break;
	case WM_LBUTTONUP:
		mouse.InQueueCommand({ MouseCommand::M_LBUTTON_UP });
		break;
	case WM_RBUTTONUP:
		mouse.InQueueCommand({ MouseCommand::M_RBUTTON_UP });
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
