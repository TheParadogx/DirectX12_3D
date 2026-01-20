#pragma once
#include"KeyCode.hpp"

#include<array>

namespace Engine::Input
{
	class Keyboard
	{
		/// <summary>
		/// キーの入力イベント
		/// </summary>
		void KeyProcess(WPARAM VKCode, bool IsDown);

		/// <summary>
		/// Windowsの仮想キーコードをkeyboard::eKeyに変換する
		/// </summary>
		/// <param name="vkCode"></param>
		/// <returns></returns>
		eKeyCode ToKeyCode(WPARAM VKCode);

		/// <summary>
		/// キーコードが範囲内か判定する
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns>true:範囲内</returns>
		bool IsValid(eKeyCode keyCode)const;
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		Keyboard();
		virtual ~Keyboard() = default;

		/// <summary>
		/// ウィンドウからイベント取得
		/// </summary>
		/// <returns>true:入力関係のイベント</returns>
		bool ProcessEvent(UINT Message, WPARAM VKCode);

		/// <summary>
		/// 押した瞬間
		/// </summary>
		/// <param name="keyCode">キーコード</param>
		/// <returns></returns>
		bool IsPressed(eKeyCode keyCode)const;

		/// <summary>
		/// 押している間
		/// </summary>
		/// <param name="keyCode">キーコード</param>
		/// <returns></returns>
		bool IsHeld(eKeyCode keyCode)const;

		/// <summary>
		/// 離した瞬間
		/// </summary>
		/// <param name="keyCode">キーコード</param>
		/// <returns></returns>
		bool IsReleased(eKeyCode keyCode)const;

		/// <summary>
		/// 状態の更新
		/// </summary>
		void Update();
	private:
		std::array<bool, static_cast<int>(eKeyCode::Count)> mCurrKeys;
		std::array<bool, static_cast<int>(eKeyCode::Count)> mPrevKeys;
	};
}