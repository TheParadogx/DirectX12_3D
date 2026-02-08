#include "pch.h"
#include "GameRuleSystem.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/GameOver/GameOverScene.hpp"
#include"Application/Scene/GameClear/GameClear.hpp"
#include"Application/Scene/StageSelect/StageSelectScene.hpp"

#include"System/CSV/CSVManager.hpp"
#include"Application/Data/SaveData.hpp"

#include"Application/Components/Enemy/Parameter/EnemyParametersComponent.hpp"
#include"System/Conponent/Timer/TimerComponent.hpp"
#include"Application/Components/Tag/TagComponent.hpp"

/// <summary>
/// Gameの終了条件を見る
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::GameRuleSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	if (mIsStateChange == true) return;

	EnemyRank rank = EnemyRank::Basic;

	//	敵が全滅しているかどうかを見る
	auto AliveEnemy = Reg.view<EnemyTag>(entt::exclude<DeadTag>);
	if (AliveEnemy.begin() == AliveEnemy.end())
	{
		//	このときに完全クリアのタグがあるかどうかを判定してクリア画面か
		//	セレクト画面に戻すかどうかの判定をする
		bool gameClear = false;
		auto bosses = Reg.view<BossTag>();
		auto Advance = Reg.view<AdvancedTag>();
		auto Basic = Reg.view<BasicTag>();
		
		//	今回攻略したレベル
		int nextLevel = 0;
		for (auto entity : bosses)
		{
			if (Reg.all_of<DeadTag>(entity))
			{
				gameClear = true;
				nextLevel = 3;
				break;
			}
		}
		if (nextLevel == 0)
		{
			for (auto entity : Advance)
			{
				if (Reg.all_of<DeadTag>(entity))
				{
					nextLevel = 2;
					break;
				}
			}
		}
		if (nextLevel == 0)
		{
			for (auto entity : Basic)
			{
				if (Reg.all_of<DeadTag>(entity))
				{
					nextLevel = 1;
					break;
				}
			}
		}


		// どのタグの敵を倒したか判定（強い順にチェック）
		// Boss判定

		if (gameClear == true)
		{
			//	タイマーの取得
			auto TimerView = Reg.view<TimerComponent, InGameTag>();
			auto TimerEntity = TimerView.front();
			double Time = 0;
			if (TimerEntity != entt::null)
			{
				auto& T = TimerView.get<TimerComponent>(TimerEntity);
				Time = T.Timer;
			}

			SceneManager::GetInstance()->ChangeSceneFade<Scene::GameClear>(Time);
		}
		else
		{
			SceneManager::GetInstance()->ChangeSceneFade<Scene::StageSelect>();
		}


		//	どの敵を射倒したかの判定

		//	値の入れ替え
		auto* data = CSV::Get<SaveData>().FindMutable(1);
		if (data != nullptr)
		{
			if (data->ClearLevel < nextLevel)
			{
				data->ClearLevel = nextLevel;
			}
		}

		mIsStateChange = true;

	}
	else
	{
		auto& param = Reg.get<EnemyParameters>(AliveEnemy.front());
		rank = param.Rank;
	}

	auto AlivePlayer = Reg.view<PlayerTag>(entt::exclude<DeadTag>);
	if (AlivePlayer.begin() == AlivePlayer.end())
	{
		//	上と同じように演出を入れたい
		SceneManager::GetInstance()->ChangeSceneFade<Scene::GameOver>(rank);
		mIsStateChange = true;
	}
}
