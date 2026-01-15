#pragma once

template<typename T>
constexpr bool IsNullptr(T* ptr) noexcept { return ptr == nullptr; }

template<typename T>
constexpr bool IsNotNullptr(T* ptr) noexcept { return ptr != nullptr; }