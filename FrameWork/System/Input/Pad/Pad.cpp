#include "pch.h"
#include "Pad.hpp"


std::string ToString(Engine::Input::ePadButton Button)
{
	switch (Button)
	{
	case Engine::Input::ePadButton::A: return "A";
	case Engine::Input::ePadButton::B: return "B";
	case Engine::Input::ePadButton::X: return "X";
	case Engine::Input::ePadButton::Y: return "Y";
	case Engine::Input::ePadButton::DPadUp: return "DpadUp";
	case Engine::Input::ePadButton::DPadDown: return "DpadDown";
	case Engine::Input::ePadButton::DPadLeft: return "DpadLeft";
	case Engine::Input::ePadButton::DPadRight: return "DpadRight";
	case Engine::Input::ePadButton::L1: return "L1";
	case Engine::Input::ePadButton::R1: return "R1";
	case Engine::Input::ePadButton::L2: return "L2";
	case Engine::Input::ePadButton::R2: return "R2";
	case Engine::Input::ePadButton::L3: return "L3";
	case Engine::Input::ePadButton::R3: return "R3";
	case Engine::Input::ePadButton::Menu: return "Menu";
	case Engine::Input::ePadButton::View: return "View";
	case Engine::Input::ePadButton::Paddle1: return "Paddle1";
	case Engine::Input::ePadButton::Paddle2: return "Paddle2";
	case Engine::Input::ePadButton::Paddle3: return "Paddle3";
	case Engine::Input::ePadButton::Paddle4: return "Paddle4";
	default: return "Unknown";
	};
};


/// <summary>
/// 状態更新
/// </summary>
void  Engine::Input::Pad::Update()
{
	//	前状態の保存
	mPrevButtons = mCurrButtons;
	mPrevTrigger = mCurrTrigger;

	//	今の入力状態更新
	auto reading = mGamepad.GetCurrentReading();

	//	ボタン
	mCurrButtons = static_cast<uint32_t>(reading.Buttons);

	//	トリガー
	mLeftTrigger = this->ApplyDeadZone(reading.LeftTrigger, mTriggerDeadZone);
	mRightTrigger = this->ApplyDeadZone(reading.RightTrigger, mTriggerDeadZone);
	mCurrTrigger[static_cast<int>(Trigger::Left)] = (mLeftTrigger > 0.0f);
	mCurrTrigger[static_cast<int>(Trigger::Right)] = (mRightTrigger > 0.0f);

	//	スティック
	mLeftStick.x = this->ApplyDeadZone(reading.LeftThumbstickX, mLeftStickDeadZone);
	mLeftStick.y = this->ApplyDeadZone(reading.LeftThumbstickY, mLeftStickDeadZone);
	mRightStick.x = this->ApplyDeadZone(reading.RightThumbstickX, mRightStickDeadZone);
	mRightStick.y = this->ApplyDeadZone(reading.RightThumbstickY, mRightStickDeadZone);
}

/// <summary>
/// デッドゾーン適応
/// </summary>
/// <param name="Value">適応前の値</param>
/// <param name="DeadZone">デッドゾーン値</param>
/// <returns>デッドゾーン値以下なら0.0</returns>
float Engine::Input::Pad::ApplyDeadZone(float Value, float DeadZone)
{
	//	小さすぎると操作が大変なので
	DeadZone = std::max(DeadZone, 0.1f);

	//	デッドゾーン値より小さかったら入力なし
	if (std::abs(Value) < DeadZone)
	{
		return 0.0f;
	}
	return Value;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="Gamepad"></param>
Engine::Input::Pad::Pad(const Gamepad& Gamepad, const float RightStickDeadZone, const float LeftStickDeadZone, const float TriggerDeadZone)
	:mGamepad(Gamepad), mRightStickDeadZone(RightStickDeadZone), mLeftStickDeadZone(LeftStickDeadZone), mTriggerDeadZone(TriggerDeadZone)
	, mCurrButtons(0)
	, mPrevButtons(0)
	, mRightTrigger(0)
	, mLeftTrigger(0)
	, mLeftStick({0.0f,0.0f})
	, mRightStick({ 0.0f,0.0f }) 
{
}


/// <summary>
/// 押した瞬間
/// </summary>
/// <param name="button">ボタン種別</param>
/// <returns>true:押した瞬間</returns>
bool Engine::Input::Pad::IsPressed(const ePadButton Button) const
{
	switch (Button)
	{
	case ePadButton::L2:
	{
		const int index = static_cast<int>(Trigger::Left);
		return (mCurrTrigger[index] == true && mPrevTrigger[index] == false);
	}
	case ePadButton::R2:
	{
		const int index = static_cast<int>(Trigger::Right);
		return (mCurrTrigger[index] == true && mPrevTrigger[index] == false);
	}
	default:
	{
		const uint32_t mask = static_cast<uint32_t>(ToWgiButton(Button));
		const bool isCurr = (mCurrButtons & mask) != 0;
		const bool isPrev = (mPrevButtons & mask) != 0;
		return (isCurr == true && isPrev == false);
	}
	}
}

/// <summary>
/// 押している間
/// </summary>
/// <param name="button"></param>
/// <returns>true:押されている</returns>
bool Engine::Input::Pad::IsHeld(const ePadButton Button) const
{
	switch (Button)
	{
	case ePadButton::L2:
	{
		const int index = static_cast<int>(Trigger::Left);
		return mCurrTrigger[index] == true;
	}
	case ePadButton::R2:
	{
		const int index = static_cast<int>(Trigger::Right);
		return mCurrTrigger[index] == true;
	}
	default:
	{
		const uint32_t mask = static_cast<uint32_t>(ToWgiButton(Button));
		return (mCurrButtons & mask) != 0;
	}
	}
}

/// <summary>
/// 離した瞬間
/// </summary>
/// <param name="button"></param>
/// <returns>true:離した瞬間</returns>
bool Engine::Input::Pad::IsReleased(const ePadButton Button) const
{
	switch (Button)
	{
	case ePadButton::L2:
	{
		const int index = static_cast<int>(Trigger::Left);
		return (mCurrTrigger[index] == false && mPrevTrigger[index] == true);
	}
	case ePadButton::R2:
	{
		const int index = static_cast<int>(Trigger::Right);
		return (mCurrTrigger[index] == false && mPrevTrigger[index] == true);
	}
	default:
	{
		const uint32_t mask = static_cast<uint32_t>(ToWgiButton(Button));
		const bool isCurr = (mCurrButtons & mask) != 0;
		const bool isPrev = (mPrevButtons & mask) != 0;
		return (isCurr == false && isPrev == true);
	}
	}
}

/// <summary>
/// 左スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::Pad::GetLeftStick3D() const
{
	return mLeftStick;
}

/// <summary>
/// 右スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::Pad::GetRightStick3D() const
{
	return mRightStick;
}

/// <summary>
/// 2D用の左スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::Pad::GetLeftStick2D() const
{
	Math::Vector2 result = mLeftStick;
	result.y *= -1;
	return result;
}

/// <summary>
/// 右スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::Pad::GetRightStick2D() const
{
	Math::Vector2 result = mRightStick;
	result.y *= -1;
	return result;
}

/// <summary>
/// 右スティックのデッドゾーン値のセット
/// </summary>
/// <param name="RightStickDeadZone"></param>
void Engine::Input::Pad::SetRightStickDeadZone(const float RightStickDeadZone)
{
	mRightStickDeadZone = RightStickDeadZone;
}

/// <summary>
/// 右スティックのデッドゾーン値のセット
/// </summary>
/// <param name="LeftStickDeadZone"></param>
void Engine::Input::Pad::SetLeftStickDeadZone(const float LeftStickDeadZone)
{
	mLeftStickDeadZone = LeftStickDeadZone;
}

/// <summary>
/// トリガーのデッドゾーン値のセット
/// </summary>
/// <param name="TriggerDeadZone"></param>
void Engine::Input::Pad::SetTriggerDeadZone(const float TriggerDeadZone)
{
	mTriggerDeadZone = TriggerDeadZone;
}

/// <summary>
/// Padの取得
/// </summary>
/// <returns></returns>
Engine::Input::Gamepad Engine::Input::Pad::GetGamepad()
{
	return mGamepad;
}

/// <summary>
/// ImGuiの更新
/// </summary>
void Engine::Input::Pad::ImGuiUpdate()
{
	//	スティック
	ImGui::Separator();
	ImGui::PushID(this);

	const auto LStick = GetLeftStick2D();
	const auto LRight = GetRightStick2D();

	ImGui::Text("Left Stick:  (%.2f, %.2f)", LStick.x, LStick.y);
	ImGui::ProgressBar((LStick.x + 1.0f) / 2.0f, ImVec2(100, 8), "X");
	ImGui::ProgressBar((LStick.y + 1.0f) / 2.0f, ImVec2(100, 8), "Y");

	ImGui::Text("Right Stick:  (%.2f, %.2f)", LRight.x, LRight.y);
	ImGui::ProgressBar((LRight.x + 1.0f) / 2.0f, ImVec2(100, 8), "X");
	ImGui::ProgressBar((LRight.y + 1.0f) / 2.0f, ImVec2(100, 8), "Y");

	//	スティックの円の可視化
	float radius = 40.f;

	//	左
	{
		ImVec2 cursor = ImGui::GetCursorScreenPos();
		ImVec2 center(cursor.x + radius, cursor.y + radius);
		ImDrawList* dl = ImGui::GetWindowDrawList();

		dl->AddCircle(center, radius,
			IM_COL32(200, 200, 200, 100), 32, 2.0f);

		ImVec2 pos(center.x + LStick.x * radius,
			center.y + LStick.y * radius);

		dl->AddCircleFilled(pos, 5.0f, IM_COL32(255, 100, 100, 255));
		ImGui::Dummy(ImVec2(radius * 2, radius * 2));

	}

	//	右
	{
		ImVec2 cursor = ImGui::GetCursorScreenPos();
		ImVec2 center(cursor.x + radius, cursor.y + radius);
		ImDrawList* dl = ImGui::GetWindowDrawList();

		dl->AddCircle(center, radius,
			IM_COL32(200, 200, 200, 100), 32, 2.0f);

		ImVec2 pos(center.x + LRight.x * radius,
			center.y + LRight.y * radius);

		dl->AddCircleFilled(pos, 5.0f, IM_COL32(100, 255, 100, 255));
		ImGui::Dummy(ImVec2(radius * 2, radius * 2));

	}

	//	トリガー
	ImGui::Text("Triggers:");
	ImGui::Text("L: %.2f", mLeftTrigger);
	ImGui::ProgressBar(mLeftTrigger, ImVec2(100, 10));
	ImGui::Text("R: %.2f", mRightTrigger);
	ImGui::ProgressBar(mRightTrigger, ImVec2(100, 10));

	// === ボタン表示 ===
	ImGui::Spacing();
	ImGui::Text("Buttons:");
	ImGui::BeginChild("buttons_child", ImVec2(0, 0), true);
	int count = static_cast<int>(ePadButton::Count);

	for (int b = 0; b < count; ++b)
	{
		ePadButton btn = static_cast<ePadButton>(b);
		bool held = this->IsHeld(btn);

		if (held == true)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 120, 120, 255));

		}

		ImGui::Text("%s", ToString(btn).c_str());

		if (held == true)
		{
			ImGui::PopStyleColor();
		}
	}

	ImGui::EndChild();

	ImGui::PopID();

}
