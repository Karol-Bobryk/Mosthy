#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER
#include <array>
#include <functional>
#include <getopt.h>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr std::string_view DEFAULT_FLAG = "f";
constexpr const char *FLAGS_OPTSTRING = "c:";
constexpr std::array<struct option, 1> LONG_OPTIONS = {
    {"command", required_argument, nullptr, 'c'}};

class ArgumentHandler {
  using Handler = std::function<void(std::vector<std::string>)>;
  using ArgFuncPair = std::pair<std::vector<std::string>, Handler>;
  std::vector<std::string> values;

  [[nodiscard]] static bool IsValidFlagGraph(char c);
  void InitializeMap();
  void ParseArg(char arg);

public:
  std::map<std::string, ArgFuncPair> flagToFunctionMap;

  ArgumentHandler(int, char **);
  ~ArgumentHandler();

  void AddHandler(const std::string &flag, Handler handler);
  void RunHandler(const std::string &flag);
  [[nodiscard]] static bool IsFlag(const std::string &str);
  [[nodiscard]] bool IsFlagSet(const std::string &str) const;
};

#endif
