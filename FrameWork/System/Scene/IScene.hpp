#pragma once
#include"System/Time/Time.hpp"

#include<string>

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
		/// メイン更新
		/// </summary>
		virtual void Update(double DeltaTime) {};

		/// <summary>
		/// 事後更新
		/// </summary>
		virtual void PostUpdate(double FixedDeltaTime) {};

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

		/// <summary>
		/// Sceneの名前を返す
		/// </summary>
		/// <returns></returns>
		virtual std::string GetSceneName() = 0;
	};

}