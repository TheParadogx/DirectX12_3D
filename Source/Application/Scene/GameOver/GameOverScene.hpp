#pragma once
#include"System/Scene/IScene.hpp"
#include"Application/Objects/EnemyRank.hpp"

namespace Engine::Scene
{

	class GameOver : public System::IScene
	{
	public:

		GameOver(System::EnemyRank EnemyRank = System::EnemyRank::Boss)
			:mEnemyRank(EnemyRank)
		{

		}

		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize()override;

		/// <summary>
		/// 事後更新
		/// </summary>
		void PostUpdate(double FixedDeltaTime)override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release()override;

		std::string GetSceneName()override
		{
			return "GameOver";
		}

	private:
		//	次どの画面に遷移するかどうかの判定用
		enum class SelectMode : int
		{
			None,
			Title,
			Retry,
		};

		SelectMode mSelectMode = SelectMode::Retry;

		/// <summary>
		/// 最後に戦った敵のランク
		/// </summary>
		System::EnemyRank mEnemyRank = System::EnemyRank::Basic;

		entt::entity mRetryEntity = entt::null;
		entt::entity mTitleEntity = entt::null;

	};
}

