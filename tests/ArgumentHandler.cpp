#include "ArgumentHandler.h"
#include "Helpers/ArgvGenerator.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <gnu-versions.h>
#include <initializer_list>
#include <string_view>

TEST_CASE("Initialization_Null_Throws", "[ArgumentHandler]") {
  REQUIRE_THROWS(ArgumentHandler(0, nullptr));
}

TEST_CASE("Initialization_InvalidBashArgs_Throws", "[ArgumentHandler]") {

  using std::string_view, std::initializer_list, std::array;

  array<initializer_list<string_view>, 4> values{
      initializer_list<string_view>{"./binary"},                   // not enough
      initializer_list<string_view>{"./binary", "-:", "argument"}, // invalid
      initializer_list<string_view>{"./binary", "-c"},             // no arg
      initializer_list<string_view>{"./binary", "-f", "aaa"} // doesnt exist
  };

  ArgvGenerator avGen(GENERATE_REF(from_range(values)));

  REQUIRE_THROWS(ArgumentHandler(avGen.GetArgc(), avGen.GetArgvs()));
}

TEST_CASE("Initialization_ValidBashArgs_Succeeds", "[ArgumentHandler]") {

  using std::string_view, std::initializer_list, std::array;

  array<initializer_list<string_view>, 2> values{
      initializer_list<string_view>{"./binary", ".", "-c", "echo 1"},

      // It is valid syntax but ".." could get misinterpreted as echo argument
      // which is incorrect and one should stray away from writing it this way
      initializer_list<string_view>{"./binary", ".", "-c", "echo", ".."},
  };

  ArgvGenerator avGen(GENERATE_REF(from_range(values)));

  REQUIRE_NOTHROW(ArgumentHandler(avGen.GetArgc(), avGen.GetArgvs()));
}
