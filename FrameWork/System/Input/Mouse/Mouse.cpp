#include "pch.h"
#include "Mouse.hpp"

/// <summary>
///	メッセージをコードに変換
/// </summary>
/// <param name="Message"></param>
/// <param name="WParam"></param>
/// <returns></returns>
Engine::Input::eMouseButton Engine::Input::Mouse::ToMouseButton(UINT Message, WPARAM WParam) const
{
	switch (Message)
	{
		//	左
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		return eMouseButton::Left;

		//	右
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		return eMouseButton::Right;

		//	中央
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		return eMouseButton::Middle;

		// サイドボタン
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		return (GET_XBUTTON_WPARAM(WParam) == XBUTTON1) ? eMouseButton::XButton1 : eMouseButton::XButton2;

	default:
		return eMouseButton::Unknown;
		break;
	}
}

void Engine::Input::Mouse::SetInputState(eMouseButton Button, bool IsDown)
{
	if (Button != eMouseButton::Unknown)
	{
		// Enumをインデックスとして安全に使用
		mCurrButtons[static_cast<int>(Button)] = IsDown;
	}
}

/// <summary>
/// 座標更新
/// </summary>
/// <param name="LParam"></param>
void Engine::Input::Mouse::UpdatePosition(LPARAM LParam)
{
	mPosition.x = static_cast<float>(LOWORD(LParam));
	mPosition.y = static_cast<float>(HIWORD(LParam));
}

/// <summary>
/// ボタンの入力が有効かどうかの判定
/// </summary>
/// <param name="Button"></param>
/// <returns>true:有効</returns>
bool Engine::Input::Mouse::IsValid(eMouseButton Button)const
{
	return Button > eMouseButton::Unknown && Button < eMouseButton::Count;
}

Engine::Input::Mouse::Mouse()
{
	mCurrButtons.fill(false);
	mPrevButtons.fill(false);
	mPosition = Math::Vector2::Zero;
	mWheel = 0.0f;
}

bool Engine::Input::Mouse::ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam)
{
	// マウス関連メッセージ（0x0200〜0x020E）なら一律で座標を更新
	if (Message >= WM_MOUSEFIRST && Message <= WM_MOUSELAST)
	{
		mPosition.x = static_cast<float>(LOWORD(LParam));
		mPosition.y = static_cast<float>(HIWORD(LParam));
	}

	// メッセージ種別ごとの個別処理
	switch (Message)
	{
	case WM_LBUTTONDOWN: case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN: case WM_XBUTTONDOWN:
		SetInputState(ToMouseButton(Message, WParam), true);
		return true;

	case WM_LBUTTONUP: case WM_RBUTTONUP:
	case WM_MBUTTONUP: case WM_XBUTTONUP:
		SetInputState(ToMouseButton(Message, WParam), false);
		return true;

	case WM_MOUSEWHEEL:
		mWheel += static_cast<float>(GET_WHEEL_DELTA_WPARAM(WParam)) / static_cast<float>(WHEEL_DELTA);
		return true;

	case WM_MOUSEMOVE:
		return true;

	default:
		return false;
	}
}

void Engine::Input::Mouse::Update()
{
	//	移動量
	mDeltaPosition = mPosition - mPrevPosition;
	//	前情報保存
	mPrevPosition = mPosition;
	mPrevButtons = mCurrButtons;
	//	ホイールリセット
	mWheel = 0.0f;
}


bool Engine::Input::Mouse::IsPressed(eMouseButton Button)
{
	if (IsValid(Button) == false) return false;

	return mCurrButtons[static_cast<int>(Button)] == true
		&& mPrevButtons[static_cast<int>(Button)] == false;
}

bool Engine::Input::Mouse::IsHeld(eMouseButton Button)
{
	if (IsValid(Button) == false) return false;

	return mCurrButtons[static_cast<int>(Button)] == true;
}

bool Engine::Input::Mouse::IsReleased(eMouseButton Button)
{
	if (IsValid(Button) == false) return false;

	return mCurrButtons[static_cast<int>(Button)] == false
		&& mPrevButtons[static_cast<int>(Button)] == true;
}

Math::Vector2 Engine::Input::Mouse::GetPosition() const
{
	return mPosition;
}

Math::Vector2 Engine::Input::Mouse::GetDeltaPosition() const
{
	return mDeltaPosition;
}

float Engine::Input::Mouse::GetWheel() const
{
	return mWheel;
}
