#pragma once

namespace Engine::System
{
	//	無敵タグ
	struct InvincibleComponet
	{
		//	無敵になれる時間
		float InviMax = 0.5f;
		//	無敵カウント用のタイマー
		float InviTimer = 0.0f;
	};
}