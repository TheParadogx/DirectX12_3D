#pragma once

#include"System/Singleton/Singleton.hpp"
#include"System/Scene/IScene.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Fade/FadeComponent.hpp"

#include<Plugin/entt/entt.hpp>
#include<memory>
#include<concepts>
#include<string>
#include<functional>

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
		/// フェード表示
		/// </summary>
		void FadeRender();

		/// <summary>
		/// 次のSceneがある場合に切り替える
		/// </summary>
		void PostPresentUpdate();

		/// <summary>
		/// 次のSceneの作成
		/// </summary>
		/// <typeparam name="...Args">引数</typeparam>
		/// <typeparam name="T">ISceneを継承いる型</typeparam>
		template<IsScene T, class... Args>
		void ChangeScene(Args&&... args)
		{
			mReloader = [args...]() mutable
				{
					return std::make_unique<T>(std::forward<Args>(args)...);
				};

			if (mScene == nullptr)
			{
				mScene = mReloader();
				return;
			}
			mNextScene = mReloader();
		}

		template<IsScene T, class... Args>
		void ChangeSceneFade(Args&&... args)
		{
			mReloader = [args...]() mutable
				{
					return std::make_unique<T>(std::forward<Args>(args)...);
				};

			if (mScene == nullptr)
			{
				mScene = mReloader();
				return;
			}
			CreateFadeEntity();
			mNextScene = mReloader();
		}

		/// <summary>
		/// フェード用のエンティティ作成
		/// </summary>
		void CreateFadeEntity();

		/// <summary>
		/// 今のSceneを読み込み直す
		/// </summary>
		void ReloadCurrentScene();

		/// <summary>
		/// 今のSceneの名前の取得
		/// </summary>
		/// <returns></returns>
		std::string CurrentSceneName();

	private:

		/// <summary>
		/// 今のSceneを再度読み込む
		/// </summary>
		std::function<std::unique_ptr<IScene>()> mReloader;

		/// <summary>
		///	今のSceneのポインタ
		/// </summary>
		std::unique_ptr<IScene> mScene = nullptr;

		/// <summary>
		/// 次のSceneのポインタ
		/// </summary>
		std::unique_ptr<IScene> mNextScene = nullptr;

		/// <summary>
		/// 今のSceneの名前
		/// </summary>
		std::string mCurrentSceneName;

		/// <summary>
		/// フェード用のレジストリ
		/// </summary>
		entt::registry mRegistry;
	};
}

