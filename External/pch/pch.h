#pragma once

// C
#include<cstdint>
#include<cassert>

// C++
#include<utility>
#include<type_traits>
#include<typeindex>
#include<typeinfo>

// STL
#include<vector>
#include<array>
#include<queue>
#include<list>
#include<map>
#include<unordered_map>
#include<set>
#include<unordered_set>

#include<string>
#include<string_view>
#include<filesystem>

#include<memory>
#include<optional>
#include<variant>
#include<concepts>
#include<coroutine>

#include<algorithm>
#include<functional>
#include<numeric>
#include<tuple>
#include<span>
#include<ranges>
#include<initializer_list>

//	Thread
#include<thread>
#include<mutex>


// IO
#include<iostream>
#include<sstream>
#include<fstream>

// Windows
#define NOMINMAX
#include<Windows.h>

// DirectX
#include<wrl/client.h>
#include<dxgi1_6.h>
#include<d3d12.h>

// 外部ライブラリ
#include<Plugin/ImGui/imgui.h>
#include<Plugin/entt/entt.hpp>

//	汎用
#include"Utility/FlagUtils.hpp"
#include"Utility/MacroHelpers.hpp"
#include"Utility/PtrUtils.hpp"
#include"Debug/spdlog/Logger.hpp"
