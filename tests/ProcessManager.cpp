#include <ProcessManager.h>
#include <catch2/catch_test_macros.hpp>
#include <unistd.h>

// This is really ugly but to be honest I have no idea how to solve it
// differently, also as of right now I don't see any way to circumvent POSIX
// signal detection
TEST_CASE("Initialization_Sleep_Succeeds", "[ProcessManager]") {
  ProcessManager pm("sleep 10000"); // TODO: race condition
  pm.StartProcess();
  REQUIRE(pm.IsProcessRunning());
}
