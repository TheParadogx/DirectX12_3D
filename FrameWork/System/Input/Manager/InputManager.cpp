#include "pch.h"
#include "InputManager.hpp"

/// <summary>
/// 初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Input::InputManager::Initialize()
{
	mPadManager = std::make_unique<PadManager>();
	mMouse = std::make_unique<Mouse>();
	mKeyboard = std::make_unique<Keyboard>();

	/*
	* デフォルトのキーコンフィグでのアクションマップの作成
	* 余裕ができたらファイルから前回の設定を引き継げるようにします
	*/

	AddAction("Select", { eKeyCode::Enter,ePadButton::A });
	AddAction("Cancel", { eKeyCode::Escape,ePadButton::B });
	AddAction("Attack", { eKeyCode::Count,ePadButton::R2,eMouseButton::Left });
	AddAction("Jump", { eKeyCode::Space,ePadButton::A });

	return true;
}

/// <summary>
/// 状態更新
/// </summary>
void Engine::Input::InputManager::Update()
{
	mPadManager->Update();
	mMouse->Update();
	mKeyboard->Update();
}

/// <summary>
/// Pad管理の取得
/// </summary>
/// <returns></returns>
Engine::Input::PadManager* Engine::Input::InputManager::GetPadManager()
{
	return mPadManager.get();
}


/// <summary>
/// ウィンドウのイベントから入力状態の取得
/// </summary>
/// <returns>true:該当イベント</returns>
bool Engine::Input::InputManager::ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam)
{
	bool ret = false;

	//	キー
	ret = mKeyboard->ProcessEvent(Message, WParam);
	if (ret == true)
	{
		return true;
	}

	//	マウス
	ret = mMouse->ProcessEvent(Message, WParam, LParam);
	if (ret == true)
	{
		return true;
	}
	//	何もなし
	return false;
}

/// <summary>
/// マウス管理の取得
/// </summary>
/// <returns></returns>
Engine::Input::Mouse* Engine::Input::InputManager::GetMouse()
{
	return mMouse.get();
}

/// <summary>
/// キーボード管理の取得
/// </summary>
/// <returns></returns>
Engine::Input::Keyboard* Engine::Input::InputManager::GetKeyboard()
{
	return mKeyboard.get();
}

/// <summary>
/// 移動軸の取得
/// </summary>
Math::Vector2 Engine::Input::InputManager::GetAxis()
{
	Math::Vector2 key = {};
	Math::Vector2 pad = {};

	//	キーボード
	if (mKeyboard->IsHeld(Input::eKeyCode::D))
	{
		key.x += 1.0f;
	}
	if (mKeyboard->IsHeld(Input::eKeyCode::A))
	{
		key.x -= 1.0f;
	}
	//	キーボード
	if (mKeyboard->IsHeld(Input::eKeyCode::W))
	{
		key.y += 1.0f;
	}
	if (mKeyboard->IsHeld(Input::eKeyCode::S))
	{
		key.y -= 1.0f;
	}

	//	PAD
	pad = mPadManager->GetLeftStick3D();

	// ベクトル合成
	Math::Vector2 vec = pad + key;
	float len = vec.SqrLength();
	if (len > 1.0f)
	{
		vec.Normalize();
	}

	return vec;
}

/// <summary>
/// アングル軸の入力量を取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::InputManager::GetLookAxis()
{
	Math::Vector2 mouse = {};
	Math::Vector2 pad = {};

	mouse = mMouse->GetDeltaPosition();
	mouse.y = -mouse.y;
	pad = mPadManager->GetRightStick3D();

	//	ベクトルの合成
	Math::Vector2 vec = pad + mouse;

	float len = vec.SqrLength();
	if (len > 0.0001f)
	{
		vec.Normalize();
	}

	return vec; 
}

/// <summary>
/// アクション名の要録
/// </summary>
/// <param name="ActionName">アクション名</param>
/// <param name="Bind">対応したキーコード群</param>
void Engine::Input::InputManager::AddAction(const std::string& ActionName, const ActionBinding& Bind)
{
	if (ActionName.empty()) return;
	mActionMaps[ActionName] = Bind;
}

/// <summary>
/// 押した瞬間
/// </summary>
/// <param name="ActionName">アクション名</param>
/// <returns>true:押した瞬間</returns>
bool Engine::Input::InputManager::IsActionPressed(const std::string& ActionName)
{
	auto it = mActionMaps.find(ActionName);
	if (it == mActionMaps.end()) return false;

	const auto& bind = it->second;

	//	どれかが入力されていたらtrue
	const bool KeyIn = mKeyboard->IsPressed(bind.key);
	const bool PadIn = mPadManager->IsPressed(bind.pad);
	const bool MouseIn = mMouse->IsPressed(bind.mouse);

	return KeyIn || PadIn || MouseIn;
}

/// <summary>
/// 押されている
/// </summary>
/// <param name="ActionName">アクション名</param>
/// <returns>true:押されている</returns>
bool Engine::Input::InputManager::IsActionHeld(const std::string& ActionName)
{
	auto it = mActionMaps.find(ActionName);
	if (it == mActionMaps.end()) return false;

	const auto& bind = it->second;

	//	どれかが入力されていたらtrue
	const bool KeyIn = mKeyboard->IsHeld(bind.key);
	const bool PadIn = mPadManager->IsHeld(bind.pad);
	const bool MouseIn = mMouse->IsHeld(bind.mouse);

	return KeyIn || PadIn || MouseIn;
}

/// <summary>
/// 離した瞬間
/// </summary>
/// <param name="ActionName">アクション名</param>
/// <returns>true:離した瞬間</returns>
bool Engine::Input::InputManager::IsActionReleased(const std::string& ActionName)
{
	auto it = mActionMaps.find(ActionName);
	if (it == mActionMaps.end()) return false;

	const auto& bind = it->second;

	//	どれかが入力されていたらtrue
	const bool KeyIn = mKeyboard->IsReleased(bind.key);
	const bool PadIn = mPadManager->IsReleased(bind.pad);
	const bool MouseIn = mMouse->IsReleased(bind.mouse);

	return KeyIn || PadIn || MouseIn;
}
