#pragma once

#include"System/Singleton/Singleton.hpp"
#include"System/Scene/IScene.hpp"

#include<memory>
#include<concepts>


namespace Engine::System
{


	template<typename T>
	concept IsScene = std::derived_from<T, IScene>;

	/// <summary>
	/// Sceneの管理をする
	/// </summary>
	class SceneManager : public Singleton<SceneManager>
	{
		GENERATE_SINGLETON_BODY(SceneManager);

	public:
		/// <summary>
		///	初期化
		/// </summary>
		/// <returns></returns>
		bool Initialize();

		/// <summary>
		/// 可変更新
		/// </summary>
		/// <param name="DeltaTime"></param>
		void Update(double DeltaTime);

		/// <summary>
		/// 固定更新
		/// </summary>
		void PostUpdate(double FixedDeltaTime);

		/// <summary>
		/// ImGuiの更新表示
		/// </summary>
		void ImGuiUpdate();

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		/// <summary>
		/// 次のSceneがある場合に切り替える
		/// </summary>
		void PostPresentUpdate();

		/// <summary>
		/// 次のSceneの作成
		/// </summary>
		/// <typeparam name="...Args">引数</typeparam>
		/// <typeparam name="T">ISceneを継承いる型</typeparam>
		template<IsScene T,typename... Args>
		void ChangeScene(Args&&... args)
		{
			if (mScene == nullptr)
			{
				mScene = std::make_unique<T>(std::forward<Args>(args)...);
				return;
			}
			mNextScene = std::make_unique<T>(std::forward<Args>(args)...);
		}


	private:
		/// <summary>
		///	今のSceneのポインタ
		/// </summary>
		std::unique_ptr<IScene> mScene = nullptr;

		/// <summary>
		/// 次のSceneのポインタ
		/// </summary>
		std::unique_ptr<IScene> mNextScene = nullptr;
	};
}

