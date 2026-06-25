#ifndef ARGV_GENERATOR
#define ARGV_GENERATOR

#include <initializer_list>
#include <string_view>
#include <vector>

class ArgvGenerator {
  std::vector<char *> argvChar;

public:
  ArgvGenerator(std::initializer_list<std::string_view> arguments);
  ~ArgvGenerator();
  char **GetArgvs();
  size_t GetArgc();
};

#endif
