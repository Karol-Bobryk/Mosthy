#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER
#include <array>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr std::array<std::string_view, 2> FLAGS = {"-f", "-c"};
constexpr std::string_view DEFAULT_FLAG = "-f";

class ArgumentHandler {
  using Handler = std::function<void(std::vector<std::string>)>;
  using ArgFuncPair = std::pair<std::vector<std::string>, Handler>;
  int count;
  std::vector<std::string> values;

  void InitializeMap();

public:
  std::map<std::string, ArgFuncPair> FlagToFunctionMap;

  ArgumentHandler(int, char *[]);
  ~ArgumentHandler();

  void AddHandler(std::string, Handler);
  void RunHandler(std::string);
  static bool IsFlag(std::string);
};

#endif
