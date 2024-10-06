#pragma once

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <TlHelp32.h>
#include <filesystem>
#include <thread>

#define TARGET "Client-Win64-Shipping.exe"
#define LIBRARY "WutheringWave.dll"

using namespace std::chrono_literals;