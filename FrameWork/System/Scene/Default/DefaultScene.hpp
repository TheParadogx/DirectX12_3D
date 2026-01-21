#pragma once
#include"System/Scene/IScene.hpp"

namespace Engine::System
{
	class DefaultScene : public IScene
	{
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		virtual bool Initialize()override;

		/// <summary>
		/// ImGuiの更新表示
		/// </summary>
		virtual void ImGuiUpdate()override;

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Release()override;
	};
}

