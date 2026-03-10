#include "ArgumentHandler.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ArgumentHandler::~ArgumentHandler() = default;

ArgumentHandler::ArgumentHandler(int argc, char **argv) {
  if (argc < 3)
    throw std::invalid_argument("Not enough arguments provided");

  for (int i = 0; i < argc; ++i)
    values.emplace_back(argv[i]);

  if (!ArgumentHandler::IsFlag(values[1])) {
    values.insert(values.begin() + 1, std::string(DEFAULT_FLAG));
  }

  InitializeMap();

  auto iter = values.begin() + 1;

  std::string flag;
  while (iter != values.end()) {

    if (ArgumentHandler::IsFlag(*iter)) {
      flag = *(iter++);
      continue;
    }

    flagToFunctionMap.at(flag).first.push_back(*(iter++));
  }
}

void ArgumentHandler::InitializeMap() {
  for (const auto &v : values) {
    // if it is a flag assign to map (if not assigned before)
    if (IsFlag(v) && flagToFunctionMap.find(v) == flagToFunctionMap.end()) {
      auto p = std::make_pair(std::vector<std::string>(), Handler());

      flagToFunctionMap.insert({v, p});
    }
  }
}

void ArgumentHandler::AddHandler(const std::string &flag, Handler handler) {
  flagToFunctionMap.at(flag).second = std::move(handler);
}

void ArgumentHandler::RunHandler(const std::string &flag) {
  if (!IsFlag(flag))
    throw std::invalid_argument("No such flag");

  // Using at so it throws in case of failure
  flagToFunctionMap.at(flag).second(flagToFunctionMap.at(flag).first);
}

bool ArgumentHandler::IsFlag(const std::string &str) {
  return (std::find(FLAGS.begin(), FLAGS.end(), str) != FLAGS.end());
}
