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
	if (mKeyboard->IsKeyHeld(Input::eKeyCode::D))
	{
		key.x += 1.0f;
	}
	if (mKeyboard->IsKeyHeld(Input::eKeyCode::A))
	{
		key.x -= 1.0f;
	}
	//	キーボード
	if (mKeyboard->IsKeyHeld(Input::eKeyCode::W))
	{
		key.y += 1.0f;
	}
	if (mKeyboard->IsKeyHeld(Input::eKeyCode::S))
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
