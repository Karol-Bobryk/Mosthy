#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

typedef struct FlagHandlerArguments {
  std::vector<std::string> args;

public:
  ~FlagHandlerArguments() = default;
  FlagHandlerArguments() {}
} FlagHandlerArguments;

typedef struct TestArgs : FlagHandlerArguments {
  int test;

  TestArgs() : TestArgs(0) {}
  TestArgs(int val) : test(val), FlagHandlerArguments() {}

} TestArgs;

class ArgumentHandler {
  using argFuncPair = std::pair<std::vector<std::string>,
                                std::function<void(FlagHandlerArguments &)>>;
  int count;
  std::vector<std::string> values;

public:
  std::map<std::string, argFuncPair> FlagToFunctionMap;

  ArgumentHandler();
  ArgumentHandler(int, char *[]);
  ~ArgumentHandler();
};

#endif
