#include "pch.h"
#include "GameClear.hpp"
#include "Factory/GameClearFactory.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/Title/TitleScene.hpp"
namespace Engine::Scene
{

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功</returns>
	bool GameClear::Initialize()
	{
		GameClearFactory::CreateBG();
		return true;
	}

	/// <summary>
	/// 事後更新
	/// </summary>
	void GameClear::PostUpdate(double FixedDeltaTime)
	{
		//	画面遷移
		if (GET_INPUT_MANAGER->IsActionPressed("Select"))
		{
			Engine::System::SceneManager::GetInstance()->ChangeSceneFade<Title>();
		}
	}

	/// <summary>
	/// 終了処理
	/// </summary>
	void GameClear::Release()
	{
		//  ローカルオブジェクトの解放
		Engine::System::EntityManager::GetInstance()->ClearLocalEntities();

	}
}