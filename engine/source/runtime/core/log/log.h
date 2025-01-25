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

        // �ÿɱ����ģ���spdlogʵ��һ��logϵͳ
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
                // critical��ʱ����ֹ���̣���runtime error����
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