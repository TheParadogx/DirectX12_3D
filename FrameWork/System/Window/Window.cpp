#include "pch.h"
#include "Window.hpp"
#include<Plugin/ImGui/imgui_impl_win32.h>

#include "Debug/spdlog/Logger.hpp"
#include "System/Input/Manager/InputManager.hpp"
#include "Utility/PtrUtils.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//	ウィンドウプロシージャー
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	//ImGui用プロシージャー用処理呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uiMsg, wParam, lParam))
	{
		return 0;
	}

	//	入力関係のイベント
	if (IsNotNullptr(GET_INPUT_MANAGER))
	{
		if (GET_INPUT_MANAGER->ProcessEvent(uiMsg, wParam, lParam))
		{
			return 0;
		}
	}

	switch (uiMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}


void Engine::System::Window::OnCreate()
{
	mWindowClass = {};
	mWindowHandle = {};
	mIsQuitMessage = false;
}

void Engine::System::Window::OnDestory()
{
	UnregisterClass(mWindowClass.lpszClassName, mWindowClass.hInstance);
}

/// <summary>
/// ウィンドウの初期化
/// </summary>
/// <param name="WindowName">ウィンドウ名</param>
/// <param name="x">x座標</param>
/// <param name="y">y座標</param>
/// <param name="setting">ウィンドウの諸々の設定</param>
/// <returns>true:成功</returns>
bool Engine::System::Window::Initialize(const char* WindowName, int x, int y, const WindowSetting& setting)
{
	//	サイズを保存
	mWidth = setting.width;
	mHeight = setting.height;
	mVirtualWidth = setting.width;
	mVirtualHeight = setting.height;


	//	ウィンドウの設定
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	mWindowClass.lpfnWndProc = WndProc;
	mWindowClass.cbClsExtra = 0;
	mWindowClass.cbWndExtra = 0;
	mWindowClass.hInstance = GetModuleHandle(nullptr);
	mWindowClass.hIcon = LoadIcon(nullptr, IDC_ARROW);
	mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	mWindowClass.lpszMenuName = nullptr;
	mWindowClass.lpszClassName = WindowName;
	mWindowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	RegisterClassEx(&mWindowClass);

	//	フルスクリーンの場合のサイズ
	if (setting.isFullscreen == true)
	{
		mWidth = GetSystemMetrics(SM_CXSCREEN);
		mHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	//	スタイルの取得
	const DWORD tWindowStyle = setting.isFullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	RECT tRect = {};
	tRect.left = 0;
	tRect.top = 0;
	tRect.right = static_cast<LONG>(mWidth);
	tRect.bottom = static_cast<LONG>(mHeight);

	//	ウィンドウの場合
	if (setting.isFullscreen == false)
	{
		AdjustWindowRect(&tRect, tWindowStyle, false);
	}

	const LONG AdjustWidth = tRect.right - tRect.left;
	const LONG AdjustHeight = tRect.bottom - tRect.top;
	// ウインドウの生成
	mWindowHandle = CreateWindow(
		mWindowClass.lpszClassName,
		WindowName,
		tWindowStyle,
		x, y,
		static_cast<int>(AdjustWidth),
		static_cast<int>(AdjustHeight),
		nullptr,
		nullptr,
		GetModuleHandle(NULL), NULL);
	if (mWindowHandle == nullptr)
	{
		LOG_CRITICAL("ウィンドウの作成に失敗");
		return false;
	}

	ShowWindow(mWindowHandle, SW_SHOW);
	UpdateWindow(mWindowHandle);

	// フォーカスを強制的に与える
	SetForegroundWindow(mWindowHandle);
	SetFocus(mWindowHandle);
    return true;
}

/// <summary>
/// 更新メッセージの取得
/// </summary>
/// <returns></returns>
void Engine::System::Window::IsUpdateMessage()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			mIsQuitMessage = true;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

/// <summary>
/// 終了メッセージ
/// </summary>
/// <returns></returns>
bool Engine::System::Window::IsQuitMessage() const
{
	return mIsQuitMessage;
}

/// <summary>
/// ウィンドウハンドルの取得
/// </summary>
/// <returns></returns>
const HWND Engine::System::Window::GetWindowHandle() const
{
	return mWindowHandle;
}
