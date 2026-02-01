#include "pch.h"
#include "GameRuleSystem.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/GameOver/GameOverScene.hpp"
#include"Application/Scene/GameClear/GameClear.hpp"
#include"Application/Scene/StageSelect/StageSelectScene.hpp"

/// <summary>
/// Gameの終了条件を見る
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::GameRuleSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	if (IsStateChange == true) return;

	//	敵が全滅しているかどうかを見る
	auto AliveEnemy = Reg.view<EnemyTag>(entt::exclude<DeadTag>);
	if (AliveEnemy.begin() == AliveEnemy.end())
	{
		//	このときに完全クリアのタグがあるかどうかを判定してクリア画面か
		//	セレクト画面に戻すかどうかの判定をする
		bool gameClear = false;
		auto bosses = Reg.view<BossTag>();
		for (auto entity : bosses)
		{
			if (Reg.all_of<DeadTag>(entity))
			{
				gameClear = true;
				break;
			}
		}
		if (gameClear == true)
		{
			SceneManager::GetInstance()->ChangeSceneFade<Scene::GameClear>();
		}
		else
		{
			SceneManager::GetInstance()->ChangeSceneFade<Scene::StageSelect>();
		}

		IsStateChange = true;

		//	今は何もなしで画面切り替え
		//	演出を使たかったらここで専用のコンポーネントを持つオブジェクトを生成すれば
		//	演出後に次の画面に進めれる。
	}

	auto AlivePlayer = Reg.view<PlayerTag>(entt::exclude<DeadTag>);
	if (AlivePlayer.begin() == AlivePlayer.end())
	{
		//	上と同じように演出を入れたい
		SceneManager::GetInstance()->ChangeSceneFade<Scene::GameOver>();
		IsStateChange = true;
	}
}
