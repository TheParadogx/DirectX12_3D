#include "pch.h"
#include "GameOverScene.hpp"
#include "Factory/CreateGameOver.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/Title/TitleScene.hpp"
#include"Application/Scene/InGame/InGameScene.hpp"
#include"System/Entity/Manager/EntityManager.hpp"

#include"System/Conponent/Sprite/Scale/ScaleComponent.hpp"

namespace Engine::Scene
{
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功</returns>
	bool GameOver::Initialize()
	{
		mRetryEntity = GameOverFactory::CreateRetry();
		mTitleEntity = GameOverFactory::CreateTitle();
		GameOverFactory::CreateBG();

		auto manager = System::EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto& RetryScale = registry.emplace<System::ScaleEffectComponent>(mRetryEntity);
		RetryScale.BaseScale = 0.7f;
		RetryScale.IsActive = true;
		auto& RetryPulse = registry.emplace<System::ScalePulseTag>(mRetryEntity);
		RetryPulse.Duration = 5.0f;
		RetryPulse.Range = 0.1f;

		auto& TitleScale = registry.emplace<System::ScaleEffectComponent>(mTitleEntity);
		TitleScale.BaseScale = 0.7f;
		TitleScale.IsActive = false;
		auto& TitlePulse = registry.emplace<System::ScalePulseTag>(mTitleEntity);
		TitlePulse.Duration = 6.0f;
		TitlePulse.Range = 0.1f;

		return true;
	}

	/// <summary>
	/// 事後更新
	/// </summary>
	void GameOver::PostUpdate(double FixedDeltaTime)
	{
		//	前の状態
		SelectMode oldMode = mSelectMode;

		//	モード変更
		if (GET_INPUT_MANAGER->IsActionPressed("Right"))
		{
			if (mSelectMode == SelectMode::Title) {
				mSelectMode = SelectMode::Retry;
				//	SEを鳴らす
			}
		}
		if (GET_INPUT_MANAGER->IsActionPressed("Left"))
		{
			if (mSelectMode == SelectMode::Retry) {
				mSelectMode = SelectMode::Title;
				//	SEを鳴らす
			}
		}

		// フラグの切り替え
		if (oldMode != mSelectMode)
		{
			auto& reg = Engine::System::EntityManager::GetInstance()->GetRegistry();

			// Retryの演出フラグ更新
			if (auto* scale = reg.try_get<Engine::System::ScaleEffectComponent>(mRetryEntity)) {
				scale->IsActive = (mSelectMode == SelectMode::Retry);
				if (scale->IsActive) scale->Timer = 0.0f; // 選択された瞬間にアニメーションを頭から再生
			}

			// Titleの演出フラグ更新
			if (auto* scale = reg.try_get<Engine::System::ScaleEffectComponent>(mTitleEntity)) {
				scale->IsActive = (mSelectMode == SelectMode::Title);
				if (scale->IsActive) scale->Timer = 0.0f;
			}
		}


		//	画面遷移
		if (GET_INPUT_MANAGER->IsActionPressed("Select"))
		{
			switch (mSelectMode)
			{
			case Engine::Scene::GameOver::SelectMode::None:
				break;
			case Engine::Scene::GameOver::SelectMode::Title:
				Engine::System::SceneManager::GetInstance()->ChangeSceneFade<Title>();
				break;
			case Engine::Scene::GameOver::SelectMode::Retry:
				Engine::System::SceneManager::GetInstance()->ChangeSceneFade<InGame>(mEnemyRank);
				break;
			default:
				break;
			}
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