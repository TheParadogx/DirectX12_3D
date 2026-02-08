#include "pch.h"
#include "GameClear.hpp"
#include "Factory/GameClearFactory.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/Title/TitleScene.hpp"

#include"System/Time/Time.hpp"

#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"

#include"System/CSV/CSVManager.hpp"
#include"Application/Data/ClearRankData.hpp"
#include"System/Conponent/Sprite/Glow/GlowComponents.hpp"

namespace Engine::Scene
{

	/// <summary>
/// データからランクの判定
/// </summary>
/// <param name="ClearTime"></param>
/// <returns></returns>
	std::string RankFac::EvaluateRankByTime(double ClearTime)
	{
		//	データの取得
		auto datas = System::CSV::Get<System::ClearRankData>().GetRawData();
		for (auto& data : datas)
		{
			if (data.ClearTime > ClearTime)
			{
				return data.Rank;
			}
		}

		return "C";
	}

	/// <summary>
	///	クリアタイムの応じたランクの表示
	/// </summary>
	/// <param name="ClearTime"></param>
	void RankFac::CreateRank(double ClearTime)
	{
		using namespace System;

		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();

		auto& trans = registry.emplace<Transform2D>(entity);
		trans.Position = { 500,680 };

		auto rank = EvaluateRankByTime(ClearTime);

		//	スコアに応じて分岐
		std::string FilePath = "Assets/Texture/Rank/" + rank + ".png";
		auto res = Graphics::TextureManager::GetInstance()->Load(FilePath);
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		sprite.Sprite.SetColor({ 1,1,1,1 });

		auto& glow = registry.emplace<GlowComponent>(entity);
		glow.BaseIntensity = 1.0f;

		auto& liner = registry.emplace<LinearShiftTag>(entity);
		liner.StartIntensity = 10.0f;
		liner.EndIntensity = 1.0f;
		liner.Duration = 5.0f;
		liner.AutoDestroy = true;

		auto& pulse = registry.emplace<PulseTag>(entity);
		pulse.Duration = 6.0f;
		pulse.Range = 1.5f;

	}



	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功</returns>
	bool GameClear::Initialize()
	{
		RankFac::CreateClearTime(mClearTime, 880, 770, 100);
		RankFac::CreateRank(mClearTime);
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


	/// <summary>
	/// コロンの生成
	/// </summary>
	/// <param name="X"></param>
	/// <param name="Y"></param>
	void RankFac::DrawColon2D(float X, float Y)
	{
		using namespace System;

		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();

		auto& trans = registry.emplace<Transform2D>(entity);
		trans.Position = { X,Y };

		std::string FilePath = "Assets/Texture/Number/Colon.png";
		auto res = Graphics::TextureManager::GetInstance()->Load(FilePath);
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		sprite.Sprite.SetColor({ 1,1,1,1 });

		auto& glow = registry.emplace<GlowComponent>(entity);
		glow.BaseIntensity = 1.0f;

		auto& pulse = registry.emplace<PulseTag>(entity);
		pulse.Duration = 6.0f;
		pulse.Range = 0.5f;

	}

	/// <summary>
	/// 2桁固定で数字のインスタンスを生成する
	/// </summary>
	/// <param name="Value">2桁の数字</param>
	/// <param name="StartX">始点Xの座標</param>
	/// <param name="StartY">始点Yの座標</param>
	void RankFac::DrawNumber2Digits(int Value, float StartX, float StartY, float Width)
	{
		using namespace System;
		
		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		int tens = Value / 10; // 10の位
		int ones = Value % 10; // 1の位


		{
			auto entity = manager->CreateEntity();

			auto& trans = registry.emplace<Transform2D>(entity);
			trans.Position = { StartX,StartY };

			std::string FilePath = "Assets/Texture/Number/" + std::to_string(tens) + ".png";
			auto res = Graphics::TextureManager::GetInstance()->Load(FilePath);
			auto& sprite = registry.emplace<SpriteComponent>(entity, res);
			sprite.Sprite.SetColor({ 1,1,1,1 });

			auto& glow = registry.emplace<GlowComponent>(entity);
			glow.BaseIntensity = 1.0f;

			auto& pulse = registry.emplace<PulseTag>(entity);
			pulse.Duration = 6.0f;
			pulse.Range = 0.5f;

		}
		
		{
			auto entity = manager->CreateEntity();

			auto& trans = registry.emplace<Transform2D>(entity);
			trans.Position = { StartX + Width,StartY };

			std::string FilePath = "Assets/Texture/Number/" + std::to_string(ones) + ".png";
			auto res = Graphics::TextureManager::GetInstance()->Load(FilePath);
			auto& sprite = registry.emplace<SpriteComponent>(entity, res);
			sprite.Sprite.SetColor({ 1,1,1,1 });

			auto& glow = registry.emplace<GlowComponent>(entity);
			glow.BaseIntensity = 1.0f;

			auto& pulse = registry.emplace<PulseTag>(entity);
			pulse.Duration = 6.0f;
			pulse.Range = 0.5f;

		}
	}
	void RankFac::CreateClearTime(float TotalSeconds, float X, float Y, float DigitWidth)
	{
		//	時間の取得
		auto time = System::Time::GetTimeDisplay(TotalSeconds);

		//	分の作成
		DrawNumber2Digits(time.Minutes, X, Y, DigitWidth);

		//	コロン
		float colon = X + (DigitWidth * 2);
		DrawColon2D(colon, Y);

		//	秒
		float secondX = colon + DigitWidth;
		DrawNumber2Digits(time.Seconds, secondX, Y, DigitWidth);

	}
}