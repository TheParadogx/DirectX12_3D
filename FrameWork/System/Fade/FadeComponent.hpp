#pragma once
#include<functional>
#include<memory>
#include"System/Scene/IScene.hpp"


namespace Engine::System
{
	enum class eFadeState
	{
		None,      // 何もしていない（透明で停止）
		FadeOut,   // 暗くしている最中（0.0 -> 1.0）
		FadeIn     // 明るくしている最中（1.0 -> 0.0）
	};

	/// <summary>
	/// フェード用のコンポーネント
	/// </summary>
	struct FadeComponent
	{
		float CurrentTime = 0.0f;
		float TotalTime = 0.5f;
		eFadeState State = eFadeState::FadeOut;
		// どのアルファから どのアルファへ行くかを保持しておくと計算が楽
		float StartAlpha = 0.0f;
		float TargetAlpha = 1.0f;
	};
}