#pragma once

#pragma warning(disable:4369 4129)

#include <SDKDDKVer.h>
#include <Windows.h>
#include <Xinput.h>
#include <D3D11.h>
#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

#include <regex>
#include <tlhelp32.h>
#include <unordered_set>

#include <nlohmann/json.hpp>

#pragma comment(lib, "Xinput.lib")

#define GAME "Client-Win64-Shipping.exe"
#define GAME_NAME "Wuthering Wave"
#define FOLDER_NAME "WutheringWave"
#define LOG_NAME "WutheringWave.log"
#define LOG_EVENT_NAME "WutheringWaveEvent.log"

#define WINDOW_CLASS "UnrealWindow"
#define WINDOW_NAME "Wuthering Waves  "

#include "settings.hpp"
#include "logger.hpp"

#define MOD_NAME "Ellohim Private Menu"

#define PRODUCT_NAME "Ellohim"

#define DISTANCE_SPAWN 10

#define CUSTOM_HUD "Madang Hese Udud Hese Teu Boga Duit Wuhan Kontol"

namespace big
{
	using namespace std::chrono_literals;
	
	template <typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

	inline HMODULE g_hmodule{};
	inline HANDLE g_main_thread{};
	inline DWORD g_main_thread_id{};
	inline std::atomic_bool g_running{ true };
}
