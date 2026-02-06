#pragma once
#include <random>
#include <utility>
#include <type_traits>


// クラス名を RandomGenerator に変更して衝突を避ける
class RandomGenerator
{
private:
    static std::mt19937& GetEngine()
    {
        static std::random_device gen;
        static std::mt19937 engine(gen());
        return engine;
    }

    RandomGenerator() = delete;
    ~RandomGenerator() = delete;
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

public:
    template <typename T>
    static T Range(T min, T max) {
        if (min > max)
        {
            std::swap(min, max);
        }

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(GetEngine());
        }
        else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(GetEngine());
        }

        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
            "int, float, double types only");
        return T{};
    }
};

// 使い勝手のためにエイリアスを置く
using Random = RandomGenerator;

