#include "function/global/global_context.h"
#include "core/log/log.h"

/*
 * 这里所有的宏在 .cpp 文件调用的时候直接include，不用在 .h 文件中include
 */

#define LOG_HELPER(LOG_LEVEL, ...) \
    runtime_global_context.log_system->print(LOG_LEVEL, "[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_DEBUG(...) LOG_HELPER(Log::LogLevel::debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(Log::LogLevel::info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(Log::LogLevel::warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(Log::LogLevel::error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(Log::LogLevel::critical, __VA_ARGS__);

#define PolitSleep(_ms) std::this_thread::sleep_for(std::chrono::milliseconds(_ms));

#define PolitNameOf(name) #name