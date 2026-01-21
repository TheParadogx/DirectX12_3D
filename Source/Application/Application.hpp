#pragma once
#include"System/Singleton/Singleton.hpp"
#include"System/Clock/Clock.hpp"
#include"System/StepTimer/StepTimer.hpp"

namespace Engine::Core
{
	class Engine;
}

namespace Engine::System
{
	class SceneManager;
	class SystemManager;
	class EntityManager;
}

namespace Engine::App
{
	class Application : public Singleton<Application>
	{
		GENERATE_SINGLETON_BODY(Application);

		/// <summary>
		/// 最初の画面の作成
		/// </summary>
		void CreateStartScene();
	public:
		/// <summary>
		/// メインループ
		/// </summary>
		void Run();
		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize();

		/// <summary>
		/// 終了処理
		/// </summary>
		void Shutdown();
	private:
		/// <summary>
		/// １フレームの更新処理
		/// </summary>
		void Tick();

		/// <summary>
		/// 事前更新
		/// </summary>
		void PreUpdate(double dt);

		/// <summary>
		/// メイン更新
		/// </summary>
		void MainUpdate(double dt);

		/// <summary>
		/// 事後更新
		/// </summary>
		void PostUpdate(double dt);

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		/// <summary>
		/// ImGUiの更新、表示
		/// </summary>
		void ImGuiUpdate();
	private:
		Core::Engine* mEngine;
		System::SceneManager* mScene;
		System::SystemManager* mSystems;
		System::EntityManager* mEntitys;

		/// <summary>
		/// 時間経過取得用
		/// </summary>
		System::Clock mClock;

		/// <summary>
		/// ループ更新用
		/// true:ゲームループ更新
		/// </summary>
		bool mIsRunning = true;
	};
}