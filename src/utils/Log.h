#pragma once


#define PRINT_INFO(...) spdlog::info(__VA_ARGS__)
#define PRINT_WARN(...) spdlog::warn(__VA_ARGS__)
#define PRINT_ERROR(...) spdlog::error(__VA_ARGS__)
#define PRINT_CRITICAL(...) spdlog::critical(__VA_ARGS__)
