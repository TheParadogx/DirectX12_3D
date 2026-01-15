#pragma once
#include "../Singleton/Singleton.hpp"
#include "../Window/Setting/WindowSettings.hpp"

#include<Windows.h>

namespace Engine::System
{
	/// <summary>
	/// ウィンドウ用クラス
	/// </summary>
	class Window : public Singleton<Window>
	{
		GENERATE_SINGLETON_BODY(Window);

		void OnCreate()override;
		void OnDestory() override;

	public:
		/// <summary>
		/// ウィンドウの初期化
		/// </summary>
		/// <param name="WindowName">ウィンドウ名</param>
		/// <param name="x">x座標</param>
		/// <param name="y">y座標</param>
		/// <param name="setting">ウィンドウの諸々の設定</param>
		/// <returns>true:成功</returns>
		bool Initialize(const char* WindowName, int x, int y,const WindowSetting& setting);
		/// <summary>
		/// 更新メッセージの取得
		/// </summary>
		/// <returns></returns>
		void IsUpdateMessage();
		/// <summary>
		/// 終了メッセージ
		/// </summary>
		/// <returns></returns>
		bool IsQuitMessage()const;
		/// <summary>
		/// ウィンドウハンドルの取得
		/// </summary>
		/// <returns></returns>
		const HWND GetWindowHandle()const;

		// 仮想解像度とスケーリング情報
		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		int GetVirtualWidth() const { return mVirtualWidth; }
		int GetVirtualHeight() const { return mVirtualHeight; }

		float GetScaleX() const { return mWidth / mVirtualWidth; }
		float GetScaleY() const { return mHeight / mVirtualHeight; }

	private:
		/// <summary>
		/// ウィンドウクラス
		/// </summary>
		WNDCLASSEX mWindowClass;
		/// <summary>
		/// ウィンドウハンドル
		/// </summary>
		HWND mWindowHandle;
		/// <summary>
		/// Windowの横幅（実サイズ）
		/// </summary>
		float mWidth;
		/// <summary>
		/// Windowの縦幅（実サイズ）
		/// </summary>
		float mHeight;
		/// <summary>
		/// ゲーム内の仮想横幅
		/// </summary>
		float mVirtualWidth;
		/// <summary>
		/// ゲーム内の仮想縦幅
		/// </summary>
		float mVirtualHeight;
		/// <summary>
		/// 終了通知
		/// </summary>
		bool mIsQuitMessage;

	};
}