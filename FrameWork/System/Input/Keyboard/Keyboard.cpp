#include "pch.h"
#include "Keyboard.hpp"

/// <summary>
/// キーの入力イベント
/// </summary>
void Engine::Input::Keyboard::KeyProcess(WPARAM VKCode, bool IsDown)
{
	//	WPARAM からenum class変換
	auto code = this->ToKeyCode(VKCode);

	//	不明なコードじゃなければ入力状態を更新する
	if (code != eKeyCode::Unknown)
	{
		mCurrKeys[static_cast<int>(code)] = IsDown;
	}
}


/// <summary>
/// Windowsの仮想キーコードをkeyboard::eKeyに変換する
/// </summary>
/// <param name="vkCode"></param>
/// <returns></returns>
Engine::Input::eKeyCode Engine::Input::Keyboard::ToKeyCode(WPARAM VKCode)
{
	switch (VKCode)
	{
		// A ~ Z
	case 'A': return eKeyCode::A;
	case 'B': return eKeyCode::B;
	case 'C': return eKeyCode::C;
	case 'D': return eKeyCode::D;
	case 'E': return eKeyCode::E;
	case 'F': return eKeyCode::F;
	case 'G': return eKeyCode::G;
	case 'H': return eKeyCode::H;
	case 'I': return eKeyCode::I;
	case 'J': return eKeyCode::J;
	case 'K': return eKeyCode::K;
	case 'L': return eKeyCode::L;
	case 'M': return eKeyCode::M;
	case 'N': return eKeyCode::N;
	case 'O': return eKeyCode::O;
	case 'P': return eKeyCode::P;
	case 'Q': return eKeyCode::Q;
	case 'R': return eKeyCode::R;
	case 'S': return eKeyCode::S;
	case 'T': return eKeyCode::T;
	case 'U': return eKeyCode::U;
	case 'V': return eKeyCode::V;
	case 'W': return eKeyCode::W;
	case 'X': return eKeyCode::X;
	case 'Y': return eKeyCode::Y;
	case 'Z': return eKeyCode::Z;

		//	数字キー
	case '0': return eKeyCode::Num0;
	case '1': return eKeyCode::Num1;
	case '2': return eKeyCode::Num2;
	case '3': return eKeyCode::Num3;
	case '4': return eKeyCode::Num4;
	case '5': return eKeyCode::Num5;
	case '6': return eKeyCode::Num6;
	case '7': return eKeyCode::Num7;
	case '8': return eKeyCode::Num8;
	case '9': return eKeyCode::Num9;

		//	特殊キー
	case VK_ESCAPE:   return eKeyCode::Escape;
	case VK_LCONTROL: return eKeyCode::LControl;
	case VK_SHIFT:   return eKeyCode::LShift;
	case VK_LMENU:    return eKeyCode::LAlt;     // VK_MENU は Alt キー
	case VK_LWIN:     return eKeyCode::LSystem;  // 左 Windows キー
	case VK_RCONTROL: return eKeyCode::RControl;
	case VK_RSHIFT:   return eKeyCode::RShift;
	case VK_RMENU:    return eKeyCode::RAlt;
	case VK_RWIN:     return eKeyCode::RSystem;  // 右 Windows キー
	case VK_APPS:     return eKeyCode::Menu;     // アプリケーションキー (メニューキー)

	case VK_SPACE:    return eKeyCode::Space;
	case VK_RETURN:   return eKeyCode::Enter;
	case VK_BACK:     return eKeyCode::Backspace;
	case VK_TAB:      return eKeyCode::Tab;
	case VK_PAUSE:    return eKeyCode::Pause;

		//	句読点
	case VK_OEM_4:    return eKeyCode::LBracket;   // [ {
	case VK_OEM_6:    return eKeyCode::RBracket;   // ] }
	case VK_OEM_1:    return eKeyCode::Semicolon;  // ; :
	case VK_OEM_COMMA:return eKeyCode::Comma;      // , <
	case VK_OEM_PERIOD: return eKeyCode::Period;   // . >
	case VK_OEM_7:    return eKeyCode::Apostrophe; // ' "
	case VK_OEM_2:    return eKeyCode::Slash;      // / ?
	case VK_OEM_5:    return eKeyCode::Backslash;  // \ |
	case VK_OEM_3:    return eKeyCode::Grave;      // ` ~
	case VK_OEM_PLUS: return eKeyCode::Equal;      // = +
	case VK_OEM_MINUS:return eKeyCode::Hyphen;     // - _

		//	ナビゲーション
	case VK_PRIOR:    return eKeyCode::PageUp;
	case VK_NEXT:     return eKeyCode::PageDown;
	case VK_END:      return eKeyCode::End;
	case VK_HOME:     return eKeyCode::Home;
	case VK_INSERT:   return eKeyCode::Insert;
	case VK_DELETE:   return eKeyCode::Delete;

	case VK_LEFT:     return eKeyCode::Left;
	case VK_RIGHT:    return eKeyCode::Right;
	case VK_UP:       return eKeyCode::Up;
	case VK_DOWN:     return eKeyCode::Down;

		//	テンキー
	case VK_NUMPAD0:  return eKeyCode::Numpad0;
	case VK_NUMPAD1:  return eKeyCode::Numpad1;
	case VK_NUMPAD2:  return eKeyCode::Numpad2;
	case VK_NUMPAD3:  return eKeyCode::Numpad3;
	case VK_NUMPAD4:  return eKeyCode::Numpad4;
	case VK_NUMPAD5:  return eKeyCode::Numpad5;
	case VK_NUMPAD6:  return eKeyCode::Numpad6;
	case VK_NUMPAD7:  return eKeyCode::Numpad7;
	case VK_NUMPAD8:  return eKeyCode::Numpad8;
	case VK_NUMPAD9:  return eKeyCode::Numpad9;
	case VK_ADD:      return eKeyCode::Add;
	case VK_SUBTRACT: return eKeyCode::Subtract;
	case VK_MULTIPLY: return eKeyCode::Multiply;
	case VK_DIVIDE:   return eKeyCode::Divide;
	case VK_DECIMAL:  return eKeyCode::Period;

		//	ファンクション
	case VK_F1:  return eKeyCode::F1;
	case VK_F2:  return eKeyCode::F2;
	case VK_F3:  return eKeyCode::F3;
	case VK_F4:  return eKeyCode::F4;
	case VK_F5:  return eKeyCode::F5;
	case VK_F6:  return eKeyCode::F6;
	case VK_F7:  return eKeyCode::F7;
	case VK_F8:  return eKeyCode::F8;
	case VK_F9:  return eKeyCode::F9;
	case VK_F10: return eKeyCode::F10;
	case VK_F11: return eKeyCode::F11;
	case VK_F12: return eKeyCode::F12;
	case VK_F13: return eKeyCode::F13;
	case VK_F14: return eKeyCode::F14;
	case VK_F15: return eKeyCode::F15;

		//	不明
	default: return eKeyCode::Unknown;
	}

}

/// <summary>
/// キーコードが範囲内か判定する
/// </summary>
/// <param name="keyCode"></param>
/// <returns>true:範囲内</returns>
bool Engine::Input::Keyboard::IsValid(eKeyCode keyCode) const
{
	return keyCode < eKeyCode::Count && keyCode > eKeyCode::Unknown;
}

/// <summary>
/// 初期化
/// </summary>
Engine::Input::Keyboard::Keyboard()
{
	mCurrKeys.fill(false);
	mPrevKeys.fill(false);
}

/// <summary>
/// ウィンドウからイベント取得
/// </summary>
/// <returns>true:入力関係のイベント</returns>
bool Engine::Input::Keyboard::ProcessEvent(UINT Message, WPARAM VKCode)
{
	//	押された
	if (Message == WM_KEYDOWN)
	{
		this->KeyProcess(VKCode, true);
		return true;
	}
	//	離された
	else if (Message == WM_KEYUP)
	{
		this->KeyProcess(VKCode, false);
		return true;
	}

	return false;
}


/// <summary>
/// 押した瞬間
/// </summary>
/// <param name="keyCode">キーコード</param>
/// <returns></returns>
bool Engine::Input::Keyboard::IsPressed(eKeyCode keyCode) const
{
	if (IsValid(keyCode) == false) return false;
	return (mCurrKeys[static_cast<int>(keyCode)] == true
		&& mPrevKeys[static_cast<int>(keyCode)] == false);
}

/// <summary>
/// 押している間
/// </summary>
/// <param name="keyCode">キーコード</param>
/// <returns></returns>
bool Engine::Input::Keyboard::IsHeld(eKeyCode keyCode) const
{
	if (IsValid(keyCode) == false) return false;
	return mCurrKeys[static_cast<int>(keyCode)] == true;
}

/// <summary>
/// 離した瞬間
/// </summary>
/// <param name="keyCode">キーコード</param>
/// <returns></returns>
bool Engine::Input::Keyboard::IsReleased(eKeyCode keyCode) const
{
	if (IsValid(keyCode) == false) return false;
	return mCurrKeys[static_cast<int>(keyCode)] == false
		&& mPrevKeys[static_cast<int>(keyCode)] == true;
}

/// <summary>
/// 状態の更新
/// </summary>
void Engine::Input::Keyboard::Update()
{
	mPrevKeys = mCurrKeys;
}
