#include "pch.h"
#include "StepTimer.hpp"

namespace Engine::System
{
	/// <summary>
	/// 経過時間の加算
	/// </summary>
	/// <param name="DeltaTime"></param>
	void FixedStepTimer::Advance(Time DeltaTime)
	{
		//	処理が重すぎて1フレームが長すぎた場合、上限でカットする
		if (DeltaTime < mMaxDeltaTime)
		{
			mAccumulator += DeltaTime;
		}
		else
		{
			mAccumulator += mMaxDeltaTime;
		}
	}

	/// <summary>
	/// 固定更新を行うかどうかの判定
	/// </summary>
	/// <returns>true:固定更新を行う</returns>
	bool FixedStepTimer::TryStep()
	{
		//	累積時間が更新可能時間を越していたら処理をする（true）
		if (mAccumulator.ToDuration() >= mFixedStep.ToDuration())
		{
			//　累積時間を1回分減らす
			mAccumulator = Time(mAccumulator.ToDuration() - mFixedStep.ToDuration());
			return true;
		}
		return false;
	}

	/// <summary>
	/// 補間係数の取得（0.0～1.0）
	/// </summary>
	/// <returns></returns>
	double FixedStepTimer::GetAlpha()const
	{
		auto numerator = static_cast<double>(mAccumulator.ToDuration().count());
		auto denominator = static_cast<double>(mFixedStep.ToDuration().count());
		return static_cast<double>(numerator / denominator);
	}

	/// <summary>
	/// 固定デルタタイムの取得
	/// </summary>
	/// <returns></returns>
	double FixedStepTimer::GetFixedDeltaTime() const
	{
		return mFixedStep.AsSeconds();
	}
}
