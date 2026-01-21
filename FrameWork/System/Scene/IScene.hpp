#pragma once
#include"System/Time/Time.hpp"

namespace Engine::System
{
	class IScene
	{
	public:
		IScene() = default;
		virtual ~IScene() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		virtual bool Initialize() { return true; };

		/// <summary>
		/// 可変更新
		/// </summary>
		virtual void Update(double DeltaTime) {};

		/// <summary>
		/// 固定更新
		/// </summary>
		virtual void FixedUpdate(double FixedDeltaTime) {};

		/// <summary>
		/// ImGuiの更新表示
		/// </summary>
		virtual void ImGuiUpdate() {};

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Render() {};

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Release() {};

	};

}