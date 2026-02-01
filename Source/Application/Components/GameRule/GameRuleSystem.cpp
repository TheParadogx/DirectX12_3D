#include "pch.h"
#include "GameRuleSystem.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/GameOver/GameOverScene.hpp"
#include"Application/Scene/GameClear/GameClear.hpp"

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
		//	今は何もなしで画面切り替え
		//	演出を使たかったらここで専用のコンポーネントを持つオブジェクトを生成すれば
		//	演出後に次の画面に進めれる。
		SceneManager::GetInstance()->ChangeSceneFade<Scene::GameClear>();
		IsStateChange = true;
	}

	auto AlivePlayer = Reg.view<PlayerTag>(entt::exclude<DeadTag>);
	if (AlivePlayer.begin() == AlivePlayer.end())
	{
		//	上と同じように演出を入れたい
		SceneManager::GetInstance()->ChangeSceneFade<Scene::GameOver>();
		IsStateChange = true;
	}
}
