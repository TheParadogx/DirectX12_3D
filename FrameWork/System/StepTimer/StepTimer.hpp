#pragma once
#include"System/Time/Time.hpp"

namespace Engine::System
{
	class FixedStepTimer
	{
	public:
		explicit FixedStepTimer(
			Time FixedStep = Time::Seconds(1.0 / 60.0),
			Time MaxDeltaTime = Time::Seconds(0.25))
			:mFixedStep(FixedStep)
			,mMaxDeltaTime(MaxDeltaTime)
			,mAccumulator(Time::Zero)
		{

		}
		
		
		virtual ~FixedStepTimer() = default;

		/// <summary>
		/// 経過時間の加算
		/// </summary>
		/// <param name="DeltaTime"></param>
		void Advance(Time DeltaTime);

		/// <summary>
		/// 固定更新を行うかどうかの判定
		/// </summary>
		/// <returns>true:固定更新を行う</returns>
		[[nodiscard]] bool TryStep();

		/// <summary>
		/// 補間係数の取得（0.0～1.0）
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] double GetAlpha() const;

		/// <summary>
		/// 固定デルタタイムの取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] double GetFixedDeltaTime()const;

	private:
		/// <summary>
		/// 固定更新の間隔
		/// </summary>
		Time mFixedStep;

		/// <summary>
		/// 1フレームの許容上限
		/// </summary>
		Time mMaxDeltaTime;

		/// <summary>
		/// 未処理の累積時間
		/// </summary>
		Time mAccumulator;
	};
}

