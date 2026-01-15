#pragma once
#include<random>
#include<utility>

namespace Engine::Math
{
	class Random
	{
	private:
		/// <summary>
		/// 乱数エンジンを保持して初期化を１回だけする。
		/// </summary>
		/// <returns></returns>
		static std::mt19937& GetEngine()
		{
			static std::random_device gen;
			static std::mt19937 engine(gen());
			return engine;
		}
	
		//	Randomクラスを生成できないように。
		Random() = delete;
		virtual ~Random() = delete;
		Random(const  Random&) = delete;
		Random& operator=(const  Random&) = delete;
	public:
		template <typename T>
		static T Range(T min, T max) {
			//	ランダム
			if (min > max)
			{
				std::swap(min, max);
			}
	
			// C++17の if constexpr で型ごとに分岐
			if constexpr (std::is_integral_v<T>) {
				// 整数型
				std::uniform_int_distribution<T> dist(min, max);
				return dist(GetEngine());
			}
			else if constexpr (std::is_floating_point_v<T>) {
				// 浮動小数点型
				std::uniform_real_distribution<T> dist(min, max);
				return dist(GetEngine());
			}
	
			// コンパイル時に型チェック
			static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
				"int,float,double以外の型が使用されています");
			return T{};
		}
	
	};

}