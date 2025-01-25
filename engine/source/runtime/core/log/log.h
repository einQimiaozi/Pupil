#pragma once

#include <stdexcept>
#include <cstdint>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Pupil {
    class Log {
    public:
        enum class LogLevel : uint8_t {
            debug,
            info,
            warn,
            error,
            critical
        };

        Log();
        ~Log();

        // 用可变参数模板加spdlog实现一个log系统
        template<typename... TARGS>
        void print(LogLevel level, TARGS&&... args) {
            switch (level) {
            case LogLevel::debug:
                logger->debug(std::forward<TARGS>(args)...);
                break;
            case LogLevel::info:
                logger->info(std::forward<TARGS>(args)...);
                break;
            case LogLevel::warn:
                logger->warn(std::forward<TARGS>(args)...);
                break;
            case LogLevel::error:
                logger->error(std::forward<TARGS>(args)...);
                break;
            case LogLevel::critical:
                logger->critical(std::forward<TARGS>(args)...);
                // critical的时候终止进程，用runtime error即可
                criticalCallback(std::forward<TARGS>(args)...);
                break;
            }
        }

        template<typename... TARGS>
        void criticalCallback(TARGS&&... args) {
            const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
            throw std::runtime_error(format_str);
        }

    private:
        std::shared_ptr<spdlog::logger> logger;
    };
}