#include "ArgumentHandler.h"
#include "Util.h"
#include <algorithm>
#include <cctype>
#include <getopt.h>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ArgumentHandler::~ArgumentHandler() = default;

ArgumentHandler::ArgumentHandler(int argc, char **argv) {
  if (argc < 3)
    // TODO: display help
    throw std::invalid_argument("Not enough arguments provided");

  int opt = 0;
  // parsing option args
  while ((opt = getopt_long(argc, argv, FLAGS_OPTSTRING, LONG_OPTIONS.data(),
                            nullptr)) != -1) {
    ParseArg(static_cast<char>(opt));
  }

  for (size_t i = optind; i < argc; ++i) {
    optarg = argv[i];
    ParseArg('f');
  }
}

void ArgumentHandler::ParseArg(char arg) {
  if (arg == '?')
    throw std::invalid_argument("Couldn't parse given argument");

  std::string strOpt(1, arg);

  auto p = std::make_pair(std::vector{std::string(optarg)}, Handler());

  flagToFunctionMap.emplace(strOpt, p);
}

void ArgumentHandler::InitializeMap() {
  for (const auto &v : values) {
    // if it is a flag assign to map (if not assigned before)
    if (IsFlag(v) && flagToFunctionMap.find(v) == flagToFunctionMap.end()) {
      auto p = std::make_pair(std::vector<std::string>(), Handler());

      flagToFunctionMap.emplace(v, p);
    }
  }
}

void ArgumentHandler::AddHandler(const std::string &flag, Handler handler) {
  flagToFunctionMap.at(flag).second = std::move(handler);
}

void ArgumentHandler::RunHandler(const std::string &flag) {
  if (!IsFlag(flag))
    throw std::invalid_argument("No such flag");
  if (!IsFlagSet(flag))
    throw std::invalid_argument("Flag not set");

  // Using at so it throws in case of failure
  flagToFunctionMap.at(flag).second(flagToFunctionMap.at(flag).first);
}

bool ArgumentHandler::IsValidFlagGraph(char c) {
  return std::isgraph(c) != 0 && !Util::IsIn(c, ':', '?', '-');
}

bool ArgumentHandler::IsFlag(const std::string &str) {
  if (str == DEFAULT_FLAG)
    return true;

  if (str.length() == 1 && !IsValidFlagGraph(str[0]))
    return false;

  auto matchOption = [&str](struct option opt) {
    bool res = (opt.name == str);

    if (str.length() == 1)
      res |= std::string(1, static_cast<char>(opt.val)) == str;

    return res;
  };

  return (std::find_if(LONG_OPTIONS.begin(), LONG_OPTIONS.end(), matchOption) !=
          LONG_OPTIONS.end());
}

bool ArgumentHandler::IsFlagSet(const std::string &str) const {
  return flagToFunctionMap.find(str) != flagToFunctionMap.end();
}
