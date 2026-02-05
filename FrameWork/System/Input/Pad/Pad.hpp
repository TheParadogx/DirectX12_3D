#pragma once
#include"PadButton.hpp"
#include"Math/Vector2/Vector2.h"

#include<cstdint>
#include<winrt/windows.gaming.input.h>
#include<array>

namespace Engine::Input
{
	//	別名
	using Gamepad = winrt::Windows::Gaming::Input::Gamepad;

	class Pad
	{
		/// <summary>
		/// デッドゾーン適応
		/// </summary>
		/// <param name="Value">適応前の値</param>
		/// <param name="DeadZone">デッドゾーン値</param>
		/// <returns>デッドゾーン値以下なら0.0</returns>
		float ApplyDeadZone(float Value, float DeadZone);

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="Gamepad"></param>
		Pad(const Gamepad& Gamepad,const float RightStickDeadZone = 0.1f, const float LeftStickDeadZone = 0.2f, const float TriggerDeadZone = 0.1f);

		virtual ~Pad() = default;

		/// <summary>
		/// 押した瞬間
		/// </summary>
		/// <param name="button">ボタン種別</param>
		/// <returns>true:押した瞬間</returns>
		bool IsPressed(const ePadButton Button) const;

		/// <summary>
		/// 押している間
		/// </summary>
		/// <param name="button"></param>
		/// <returns>true:押されている</returns>
		bool IsHeld(const ePadButton Button) const;

		/// <summary>
		/// 離した瞬間
		/// </summary>
		/// <param name="button"></param>
		/// <returns>true:離した瞬間</returns>
		bool IsReleased(const ePadButton Button) const;

		/// <summary>
		/// 3D用の左スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetLeftStick3D()const;

		/// <summary>
		/// 3D用の右スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetRightStick3D()const;

		/// <summary>
		/// 2D用の左スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetLeftStick2D()const;

		/// <summary>
		/// 右スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetRightStick2D()const;


		/// <summary>
		/// 状態更新
		/// </summary>
		void Update();

		/// <summary>
		/// 右スティックのデッドゾーン値のセット
		/// </summary>
		/// <param name="RightStickDeadZone"></param>
		void SetRightStickDeadZone(const float RightStickDeadZone);

		/// <summary>
		/// 右スティックのデッドゾーン値のセット
		/// </summary>
		/// <param name="LeftStickDeadZone"></param>
		void SetLeftStickDeadZone(const float LeftStickDeadZone);

		/// <summary>
		/// トリガーのデッドゾーン値のセット
		/// </summary>
		/// <param name="TriggerDeadZone"></param>
		void SetTriggerDeadZone(const float TriggerDeadZone);

		/// <summary>
		/// Padの取得
		/// </summary>
		/// <returns></returns>
		Gamepad GetGamepad();

		/// <summary>
		/// ImGuiの更新
		/// </summary>
		void ImGuiUpdate();

	private:
		enum class Trigger : int
		{
			Left = 0,
			Right,
			Count,
		};


		/// <summary>
		/// Gamepad
		/// </summary>
		Gamepad mGamepad;

		/// <summary>
		/// 左スティック-1.0~1.0
		/// </summary>
		Math::Vector2 mLeftStick;

		/// <summary>
		/// 右スティック-1.0~1.0
		/// </summary>
		Math::Vector2 mRightStick;

		/// <summary>
		///	右スティックのデッドゾーン
		/// </summary>
		float mLeftStickDeadZone;

		/// <summary>
		///	右スティックのデッドゾーン
		/// </summary>
		float mRightStickDeadZone;

		/// <summary>
		/// 左トリガー 0.0~1.0
		/// </summary>
		float mLeftTrigger;

		/// <summary>
		/// 右トリガー 0.0~1.0
		/// </summary>
		float mRightTrigger;

		/// <summary>
		/// 今のトリガー入力状態
		/// </summary>
		std::array<bool, static_cast<int>(Trigger::Count)> mCurrTrigger;

		/// <summary>
		/// 前のトリガーの入力状態
		/// </summary>
		std::array<bool, static_cast<int>(Trigger::Count)> mPrevTrigger;

		/// <summary>
		/// トリガーのデッドゾーン
		/// </summary>
		float mTriggerDeadZone;

		/// <summary>
		/// 今のボタンの状態
		/// </summary>
		uint32_t mCurrButtons;

		/// <summary>
		/// 前のボタンの状態
		/// </summary>
		uint32_t mPrevButtons;
	};
}


