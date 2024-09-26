#pragma once

#include <format>
#include <iostream>


template <typename... Args>
inline void Print(const std::string& message, Args&&... args)
{
	std::string result = std::vformat(message, std::make_format_args(std::forward<Args>(args)...));
	std::cout << result << '\n';
}


#define LOG_INFO(msg, ...)  Print(std::format("[INFO] {}", msg), __VA_ARGS__)
#define LOG_WARN(msg, ...)  Print(std::format("[WARN] {}", msg), __VA_ARGS__)
#define LOG_ERROR(msg, ...) Print(std::format("[ERROR] {}", msg), __VA_ARGS__)


#ifdef _DEBUG
	#define DEBUGBREAK() __debugbreak()
	#define ASSERT(condition, ...) { if(!condition) { LOG_ERROR(__VA_ARGS__); DEBUGBREAK(); } }
#else
	#define DEBUGBREAK()
	#define ASSERT(condition, ...)
#endif
