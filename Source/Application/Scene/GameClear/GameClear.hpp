#pragma once
#include"System/Scene/IScene.hpp"

#include<string>

namespace Engine::Scene
{

	class RankFac
	{
		/// <summary>
		/// データからランクの判定
		/// </summary>
		/// <param name="ClearTime"></param>
		/// <returns></returns>
		static std::string EvaluateRankByTime(double ClearTime);


		/// <summary>
	/// コロンの生成
	/// </summary>
	/// <param name="X"></param>
	/// <param name="Y"></param>
		static void DrawColon2D(float X, float Y);

		/// <summary>
		/// 2桁固定で数字のインスタンスを生成する
		/// </summary>
		/// <param name="Value">2桁の数字</param>
		/// <param name="StartX">始点Xの座標</param>
		/// <param name="StartY">始点Yの座標</param>
		static void DrawNumber2Digits(int Value, float StartX, float StartY, float Width);


	public:

			/// <summary>
			///	クリアタイムの応じたランクの表示
			/// </summary>
			/// <param name="ClearTime"></param>
		static void CreateRank(double ClearTime);


		/// <summary>
		/// 00:00方式で表示する
		/// </summary>
		/// <param name="TotalSeconds"></param>
		/// <param name="X"></param>
		/// <param name="Y"></param>
		/// <param name="DigitWidth"></param>
		static void CreateClearTime(float TotalSeconds, float X, float Y, float DigitWidth);


	};

	class GameClear : public System::IScene
	{
	public:
		GameClear(double ClearTime = 0)
			:mClearTime(ClearTime)
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
			return "GameClear";
		}


	private:


		double mClearTime = 0;
	};
}

