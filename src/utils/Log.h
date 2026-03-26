#pragma once

#define PRINT_INFO(...)                            \
    do {                                           \
        spdlog::get("console")->info(__VA_ARGS__); \
        spdlog::get("file")->info(__VA_ARGS__);    \
    } while (0)
#define PRINT_WARN(...)                            \
    do {                                           \
        spdlog::get("console")->warn(__VA_ARGS__); \
        spdlog::get("file")->warn(__VA_ARGS__);    \
    } while (0)
#define PRINT_ERROR(...)                            \
    do {                                            \
        spdlog::get("console")->error(__VA_ARGS__); \
        spdlog::get("file")->error(__VA_ARGS__);    \
    } while (0)
#define PRINT_CRITICAL(...)                            \
    do {                                               \
        spdlog::get("console")->critical(__VA_ARGS__); \
        spdlog::get("file")->critical(__VA_ARGS__);    \
    } while (0)
