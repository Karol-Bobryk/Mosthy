#ifndef LOGGER_SERVICE
#define LOGGER_SERVICE

#include "LogSerializable.h"
#include "ThreadPool/ThreadPool.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>

enum class LogSeverity { INFO, WARNING, CRITICAL };

class LogSeverityCast {
public:
  static constexpr std::string_view ToString(const LogSeverity &severity) {
    switch (severity) {
    case LogSeverity::INFO:
      return "INFO";
    case LogSeverity::WARNING:
      return "WARNING";
    case LogSeverity::CRITICAL:
      return "CRITICAL";
    default:
      throw std::runtime_error("Unknown log severity provided");
    }
  }
};

class LoggerService {
  ThreadPool threadPool;
  std::ofstream logOutput;

  LoggerService() : threadPool(THREAD_POOL_SIZE), logOutput(LOG_FILE) {
    threadPool.AddWorker(THREAD_POOL_SIZE);
  };

  static void LogThreadWrapper(LogSerializable &logObj, LoggerService &service,
                               LogSeverity severity);

public:
  static LoggerService &GetInstance() {
    static LoggerService instance;
    return instance;
  }

  LoggerService(LoggerService const &obj) = delete;
  void operator=(LoggerService const &obj) = delete;

  static void Log(LogSerializable &obj);
  static void Log(LogSerializable &obj, LogSeverity severity);

  // CONSTANTS
  static constexpr size_t THREAD_POOL_SIZE = 5;
  inline static const std::filesystem::path LOG_FILE = "mosthy.log";
};

#endif
