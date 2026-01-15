#pragma once
#include <math.h>
#include <concepts>

/// <summary>
/// 値の比較可能
/// </summary>
template<typename T>
concept Comparison = requires(T a, T b)
{
	{ a < b } -> std::convertible_to<T>;
}&&
	requires(const T a, const T b)
{
	{ a < b } -> std::convertible_to<T>;
}&&
	requires(const T& a, const T& b)
{
	{ a < b } -> std::convertible_to<T>;
};


namespace Math
{
	/// <summary>
	/// 円周率
	/// </summary>
	constexpr float PI = 3.14159265358979f;

	/// <summary>
	/// ラジアン角
	/// </summary>
	constexpr float RAD = 0.0174532925199433f;

	/// <summary>
	/// 比較し大きい値を返す
	/// </summary>
	/// <typeparam name="T">数値型</typeparam>
	/// <param name="value1">値1</param>
	/// <param name="value2">値2</param>
	/// <returns>大きい方の値</returns>
	template<Comparison T>
	inline T Max(const T value1, const T value2)
	{
		if (value1 > value2)
		{
			return value1;
		}
		return value2;
	}

	template<typename T>
	inline T Min(const T value1, const T value2)
	{
		if (value1 < value2)
		{
			return value1;
		}
		return value2;
	}

	template<typename T>
	inline T Clamp(const T value,const T minimum, const T maximum)
	{
		if (value > maximum)
		{
			return maximum;
		}
		if (value < minimum)
		{
			return minimum;
		}
		return value;
	}

	template<typename T>
	inline T Saturate(const T value)
	{
		return Clamp(value, static_cast<T>(0), static_cast<T>(1));
	}

	template<typename T>
	inline T Lerp(const T in1, const T in2, const float t)
	{
		const float u = 1.0f - t;
		return in1 * u + in2 * t;
	}

	template<typename T>
	inline T Pow(const T value, const int right)
	{
		T num = static_cast<T>(1);
		for (int i = 0; i < right; i++)
		{
			num *= value;
		}
		return num;
	}

	template<typename T, typename U>
	inline T Pow(const T value, const U right)
	{
		return (T)powf((T)value, (U)right);
	}

}

