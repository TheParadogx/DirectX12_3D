#pragma once

#include"System/Singleton/Singleton.hpp"
#include"EnumInputDevice.hpp"
#include "System/Input/Keyboard/Keyboard.hpp"
#include "System/Input/Mouse/Mouse.hpp"
#include "System/Input/Pad/PadManager.hpp"

#include<memory>
#include<unordered_map>
#include<string>

#ifdef GET_INPUT_MANAGER
#undef GET_INPUT_MANAGER 
#endif // GET_INPUT_MANAGER

#define GET_INPUT_MANAGER Engine::Input::InputManager::GetInstance()

namespace Engine::Input
{
	/// <summary>
	/// 各入力の管理
	/// </summary>
	class InputManager : public Singleton<InputManager>
	{
		GENERATE_SINGLETON_BODY(InputManager);

		/// <summary>
		/// アクション名に紐づける各デバイスのコード
		/// </summary>
		struct ActionBinding
		{
			eKeyCode key = eKeyCode::Count;
			ePadButton pad = ePadButton::Count;
			eMouseButton mouse = eMouseButton::Count;
		};

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

		/// <summary>
		/// アクション名の要録
		/// </summary>
		/// <param name="ActionName">アクション名</param>
		/// <param name="Bind">対応したキーコード群</param>
		void AddAction(const std::string& ActionName, const ActionBinding& Bind);

		/// <summary>
		/// 押した瞬間
		/// </summary>
		/// <param name="ActionName">アクション名</param>
		/// <returns>true:押した瞬間</returns>
		bool IsActionPressed(const std::string& ActionName);

		/// <summary>
		/// 押されている
		/// </summary>
		/// <param name="ActionName">アクション名</param>
		/// <returns>true:押されている</returns>
		bool IsActionHeld(const std::string& ActionName);

		/// <summary>
		/// 離した瞬間
		/// </summary>
		/// <param name="ActionName">アクション名</param>
		/// <returns>true:離した瞬間</returns>
		bool IsActionReleased(const std::string& ActionName);


	private:
		/// <summary>
		/// アクション毎の入力
		/// </summary>
		std::unordered_map<std::string, ActionBinding> mActionMaps;

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


