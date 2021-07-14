#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::logger;

std::shared_ptr<spdlog::logger> Logger::Init()
{
   std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("RENDERER");
   logger->set_level(spdlog::level::level_enum::trace);
   logger->set_pattern("%^[%T] %n: %v%$");
   return logger;
}