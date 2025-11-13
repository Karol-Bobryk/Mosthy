#include "ArgumentHandler.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

ArgumentHandler::~ArgumentHandler() {}

ArgumentHandler::ArgumentHandler(int argc, char *argv[]) {
  if (argc < 3)
    throw std::invalid_argument("Not enough arguments provided");

  for (int i = 0; i < argc; ++i)
    values.push_back(argv[i]);

  if (!ArgumentHandler::IsFlag(values[1])) {
    values.insert(values.begin() + 1, std::string(DEFAULT_FLAG));
  }

  InitializeMap();

  auto iter = values.begin() + 1;

  std::string flag = "";
  while (iter != values.end()) {

    if (ArgumentHandler::IsFlag(*iter)) {
      flag = *(iter++);
      continue;
    }

    FlagToFunctionMap.at(flag).first.push_back(*(iter++));
  }
}

void ArgumentHandler::InitializeMap() {
  for (const auto &v : values) {
    // if it is a flag assign to map (if not assigned before)
    if (IsFlag(v) && FlagToFunctionMap.find(v) == FlagToFunctionMap.end()) {
      auto p = std::make_pair(std::vector<std::string>(), Handler());

      FlagToFunctionMap.insert({v, p});
    }
  }
}

void ArgumentHandler::AddHandler(std::string flag, Handler handler) {
  FlagToFunctionMap.at(flag).second = handler;
}

void ArgumentHandler::RunHandler(std::string flag) {
  if (!IsFlag(flag))
    throw std::invalid_argument("No such flag");

  // Using at so it throws in case of failure
  FlagToFunctionMap.at(flag).second(FlagToFunctionMap.at(flag).first);
}

bool ArgumentHandler::IsFlag(std::string str) {
  return (std::find(FLAGS.begin(), FLAGS.end(), str) != FLAGS.end());
}
