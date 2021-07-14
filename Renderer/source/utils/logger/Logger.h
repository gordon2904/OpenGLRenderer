#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
   static std::shared_ptr<spdlog::logger> Init();
   inline static std::shared_ptr<spdlog::logger>& GetLogger() 
   {
      if(logger == nullptr)
      {
         logger = Init();
      }
      return logger;
   }
protected:
   static std::shared_ptr<spdlog::logger> logger;
};


#ifdef _DEBUG
#define LOG_INFO(...) ::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#else
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_TRACE(...)
#endif


