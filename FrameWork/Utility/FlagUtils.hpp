#pragma once
#include<concepts>
#include <type_traits>

//	整数型のみ
template<typename T>
concept Integral = std::is_integral_v<T>;

/// <summary>
/// フラグが全て立っているかどうか
/// </summary>
/// <typeparam name="T">整数型</typeparam>
/// <param name="Value">ビット列</param>
/// <param name="Flag">確認したいビット</param>
/// <returns>true:全て立っている</returns>
template<Integral T>
constexpr bool HasFlag(T Value, T Flag)noexcept
{
	return (Value & Flag) == Flag;
}

/// <summary>
/// １つでもフラグが立っているかどうか
/// </summary>
/// <typeparam name="T">整数型</typeparam>
/// <param name="Value">ビット列</param>
/// <param name="Flag">確認したいビット</param>
/// <returns>true:立っている</returns>
template<Integral T>
constexpr bool HasAnyFlag(T Value, T Flag) noexcept
{
	return (Value & Flag) != 0;
}

/// <summary>
/// フラグを立てる
/// </summary>
/// <typeparam name="T">整数型</typeparam>
/// <param name="Value">ビット列</param>
/// <param name="Flag">立てたいビット</param>
template<Integral T>
constexpr void SetFlag(T& Value, T Flag) noexcept
{
	Value |= Flag;
}

/// <summary>
/// フラグを下げる 
/// </summary>
/// <typeparam name="T">整数型</typeparam>
/// <param name="Value">ビット列</param>
/// <param name="Flag">下げたいビッド</param>
template<Integral T>
constexpr void ClearFlag(T& Value, T Flag) noexcept
{
	Value &= ~Flag;
}

/// <summary>
///フラグを反転（トグル） 
/// </summary>
/// <typeparam name="T">整数型</typeparam>
/// <param name="Value">ビット列</param>
/// <param name="Flag">反転させたいビット</param>
template<Integral T>
constexpr void ToggleFlag(T& Value, T Flag) noexcept
{
	Value ^= Flag;
}


#define DEFINE_ENUM_BITWISE_OPERATORS(Type) \
inline Type operator| (Type a, Type b) { \
    return static_cast<Type>(static_cast<std::underlying_type_t<Type>>(a) | static_cast<std::underlying_type_t<Type>>(b)); \
} \
inline Type operator& (Type a, Type b) { \
    return static_cast<Type>(static_cast<std::underlying_type_t<Type>>(a) & static_cast<std::underlying_type_t<Type>>(b)); \
} \
inline Type operator^ (Type a, Type b) { \
    return static_cast<Type>(static_cast<std::underlying_type_t<Type>>(a) ^ static_cast<std::underlying_type_t<Type>>(b)); \
} \
inline Type operator~ (Type a) { \
    return static_cast<Type>(~static_cast<std::underlying_type_t<Type>>(a)); \
} \
inline Type& operator|= (Type& a, Type b) { \
    return a = a | b; \
} \
inline Type& operator&= (Type& a, Type b) { \
    return a = a & b; \
} \
inline Type& operator^= (Type& a, Type b) { \
    return a = a ^ b; \
} \
/* 判定用ヘルパー (if (a & b) を可能にするため) */ \
inline bool operator!(Type a) { \
    return static_cast<std::underlying_type_t<Type>>(a) == 0; \
}
template<typename T>
inline bool HasFlag(T alpha, T beta) {
    return static_cast<std::underlying_type_t<T>>(alpha & beta) != 0;
}