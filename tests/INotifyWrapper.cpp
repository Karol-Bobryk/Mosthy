#include "INotifyWrapper.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("INotifyWrapper initialization", "[INotifyWrapped]") {
  INotifyWrapper intfw;
  REQUIRE(intfw.IsInstanceGood());
}
