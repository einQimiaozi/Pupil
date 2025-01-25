#include "log.h"

namespace Pupil {
    Log::Log() {
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);
        console_sink->set_pattern("[%^%l%$] %v");

        const spdlog::sinks_init_list sink_list = { console_sink };

        spdlog::init_thread_pool(8192, 1);

        logger = std::make_shared<spdlog::async_logger>("logger",
            sink_list.begin(),
            sink_list.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);
        logger->set_level(spdlog::level::trace);

        spdlog::register_logger(logger);
    }

    Log::~Log() {
        logger->flush();
        spdlog::drop_all();
    }
}