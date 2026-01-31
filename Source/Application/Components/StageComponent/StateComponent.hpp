#pragma once

namespace Engine::System
{
	/// <summary>
	/// 移動制限などの情報を持つステージの地面用のコンポーネント
	/// </summary>
	struct StageComponent
	{
		/// <summary>
		/// プレイできるステージの範囲
		/// </summary>
		float PlayableRadius = 150.0f;
	};
}