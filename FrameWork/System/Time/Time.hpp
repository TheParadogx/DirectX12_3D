#pragma once
#include<chrono>
#include<stdint.h>

namespace Engine::System 
{

	// タイムを分解する構造体（使い勝手がいいように）
	struct TimeDisplay {
		int Minutes;
		int Seconds;
	};

	class Time
	{
	public:
		//	ゼロ初期化
		constexpr Time() = default;

		template <typename Rep, typename Period>
		explicit constexpr Time(const std::chrono::duration<Rep, Period>& duration)
			: mMicroseconds(
				std::chrono::duration_cast<std::chrono::microseconds>(duration))
		{
		}

		//	ファクトリ
		[[nodiscard]] static constexpr Time Seconds(double Seconds)
		{
			return Time(std::chrono::duration<double>(Seconds));
		}

		[[nodiscard]] static constexpr Time Milliseconds(int64_t Ms)
		{
			return Time(std::chrono::milliseconds(Ms));
		}

		//	アクセサ
		[[nodiscard]] constexpr double AsSeconds()const
		{
			return std::chrono::duration<double>(mMicroseconds).count();
		}
		[[nodiscard]] constexpr std::int64_t AsMilliseconds()const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(mMicroseconds).count();
		}
		[[nodiscard]] constexpr  std::chrono::microseconds ToDuration()const
		{
			return mMicroseconds;
		}

		/// <summary>
		/// 時間を分と秒に変換する。
		/// </summary>
		/// <param name="totalSeconds"></param>
		/// <returns></returns>
		static TimeDisplay GetTimeDisplay(float totalSeconds) {
			int total = static_cast<int>(totalSeconds);
			int m = total / 60;
			int s = total % 60;
			return { m, s };
		}


		// 比較
		constexpr bool operator==(const Time& Other) const = default;
		constexpr bool operator<(const Time& Other) const
		{
			return mMicroseconds < Other.mMicroseconds;
		}

		// 加算（必要最小限）
		constexpr Time operator+(const Time& Other) const
		{
			return Time(mMicroseconds + Other.mMicroseconds);
		}

		constexpr Time& operator+=(const Time& Other)
		{
			mMicroseconds += Other.mMicroseconds;
			return *this;
		}

		// 定数
		static const Time Zero;
	private:
		std::chrono::microseconds mMicroseconds = {};

	};
}