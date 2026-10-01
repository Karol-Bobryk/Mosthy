#include "Logger/LoggerService.h"
#include "Logger/LogSerializable.h"
#include <chrono>
#include <ctime>
#include <functional>
#include <sstream>

void LoggerService::Log(LogSerializable &obj) {
  LoggerService::Log(obj, LogSeverity::INFO);
}

void LoggerService::Log(LogSerializable &obj, LogSeverity severity) {
  // TODO: add threading

  LoggerService &service = GetInstance();

  service.threadPool.AddTask(LoggerService::LogThreadWrapper, std::ref(obj),
                             std::ref(service), severity);
}

void LoggerService::LogThreadWrapper(LogSerializable &logObj,
                                     LoggerService &service,
                                     LogSeverity severity) {
  using namespace std::chrono;

  auto tsNow = system_clock::now();

  std::time_t t = system_clock::to_time_t(tsNow);

  std::stringstream sstr;
  sstr << std::ctime(&t) << " [ " << LogSeverityCast::ToString(severity)
       << " ] " << logObj.GetLogString();
  service.logOutput << sstr.str();
}
