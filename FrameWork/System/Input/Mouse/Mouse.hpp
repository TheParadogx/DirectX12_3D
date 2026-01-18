#pragma once

#include"MouseButton.hpp"
#include"Math/Vector2/Vector2.h"

#include<array>

namespace Engine::Input
{
	class Mouse
	{
		/// <summary>
		///	メッセージをコードに変換
		/// </summary>
		/// <param name="Message"></param>
		/// <param name="WParam"></param>
		/// <returns></returns>
		eMouseButton ToMouseButton(UINT Message, WPARAM WParam)const;

		/// <summary>
		/// 入力状態を更新する
		/// </summary>
		/// <param name="Button"></param>
		/// <param name="IsDown"></param>
		void SetInputState(eMouseButton Button, bool IsDown);

		/// <summary>
		/// 座標更新
		/// </summary>
		/// <param name="LParam"></param>
		void UpdatePosition(LPARAM LParam);

		/// <summary>
		/// ボタンの入力が有効かどうかの判定
		/// </summary>
		/// <param name="Button"></param>
		/// <returns>true:有効</returns>
		bool IsValid(eMouseButton Button)const;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		Mouse();
		virtual ~Mouse() = default;

		bool ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam);
		void Update();

		bool IsPressed(eMouseButton Button);
		bool IsHeld(eMouseButton Button);
		bool IsReleased(eMouseButton Button);

		Math::Vector2 GetPosition()const;
		Math::Vector2 GetDeltaPosition() const;
		float GetWheel()const;

	private:
		/// <summary>
		/// 入力状態
		/// </summary>
		std::array<bool, static_cast<int>(eMouseButton::Count)> mCurrButtons;
		std::array<bool, static_cast<int>(eMouseButton::Count)> mPrevButtons;

		/// <summary>
		/// 座標
		/// </summary>
		Math::Vector2 mPosition;
		/// <summary>
		/// 前フレームの座標
		/// </summary>
		Math::Vector2 mPrevPosition; 
		/// <summary>
		/// 1フレーム間の移動量
		/// </summary>
		Math::Vector2 mDeltaPosition;

		/// <summary>
		/// マウスホイール
		/// </summary>
		float mWheel;
	};
}


