#pragma once

#include"System/Singleton/Singleton.hpp"
#include"EnumInputDevice.hpp"
#include "System/Input/Keyboard/Keyboard.hpp"
#include "System/Input/Mouse/Mouse.hpp"
#include "System/Input/Pad/PadManager.hpp"

#include<memory>

#ifdef GET_INPUT_MANAGER
#undef GET_INPUT_MANAGER 
#endif // GET_INPUT_MANAGER

#define GET_INPUT_MANAGER Engine::Input::InputManager::GetInstance()

namespace Engine::Input
{

	enum class Axis
	{
		Horizontal,
		Vertical,
	};

	class InputManager : public Singleton<InputManager>
	{
		GENERATE_SINGLETON_BODY(InputManager);

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize();

		/// <summary>
		/// 状態更新
		/// </summary>
		void Update();

		/// <summary>
		/// ウィンドウのイベントから入力状態の取得
		/// </summary>
		/// <returns>true:該当イベント</returns>
		bool ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam);

		/// <summary>
		/// Pad管理の取得
		/// </summary>
		/// <returns></returns>
		PadManager* GetPadManager();

		/// <summary>
		/// マウス管理の取得
		/// </summary>
		/// <returns></returns>
		Mouse* GetMouse();

		/// <summary>
		/// キーボード管理の取得
		/// </summary>
		/// <returns></returns>
		Keyboard* GetKeyboard();

		/// <summary>
		/// 移動軸の入力量の取得
		/// </summary>
		/// <param name="Name"></param>
		/// <returns></returns>
		Math::Vector2 GetAxis();

		/// <summary>
		/// アングル軸の入力量を取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetLookAxis();

	private:
		/// <summary>
		/// Pad管理のインスタンス
		/// </summary>
		std::unique_ptr<PadManager> mPadManager;
		/// <summary>
		/// マウス管理のインスタンス
		/// </summary>
		std::unique_ptr<Mouse> mMouse;
		/// <summary>
		/// キーボード管理のインスタンス
		/// </summary>
		std::unique_ptr<Keyboard> mKeyboard;
	};
}


