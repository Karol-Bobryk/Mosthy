#ifndef TEST_HELPER
#define TEST_HELPER

#include <initializer_list>
#include <string_view>
class ArgvGenerator {
  ArgvGenerator(std::initializer_list<std::string_view> arguments);
};

#endif
