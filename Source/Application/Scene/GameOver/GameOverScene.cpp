#include "pch.h"
#include "GameOverScene.hpp"
#include "Factory/CreateGameOver.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/Title/TitleScene.hpp"
#include"Application/Scene/InGame/InGameScene.hpp"

namespace Engine::Scene
{
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功</returns>
	bool GameOver::Initialize()
	{
		GameOverFactory::CreateBG();
		return true;
	}

	/// <summary>
	/// 事後更新
	/// </summary>
	void GameOver::PostUpdate(double FixedDeltaTime)
	{
		//	後で再戦機能も実装します

		if (GET_INPUT_MANAGER->IsActionPressed("Select"))
		{
			Engine::System::SceneManager::GetInstance()->ChangeSceneFade<Title>();
		}
	}

	/// <summary>
	/// 終了処理
	/// </summary>
	void GameOver::Release()
	{
		//  ローカルオブジェクトの解放
		Engine::System::EntityManager::GetInstance()->ClearLocalEntities();
	}
}