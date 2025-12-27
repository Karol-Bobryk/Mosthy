#include "INotifyWrapper.h"
#include <catch2/catch_test_macros.hpp>
#include <sys/inotify.h>

TEST_CASE("Initialization_NoArgs_Succeeds", "[INotifyWrapped]") {
  INotifyWrapper intfw;
  REQUIRE(intfw.IsInstanceGood());
}

TEST_CASE("Initialization_FallbackFlags_Succeeds", "[INotifyWrapped]") {

  SECTION("No flags") {
    INotifyWrapper intfw(0);

    REQUIRE(intfw.IsInstanceGood());
  }

  SECTION("Modify flag") {
    INotifyWrapper intfw(IN_MODIFY);

    REQUIRE(intfw.IsInstanceGood());
  }

  SECTION("Ignore flag") {
    INotifyWrapper intfw(IN_IGNORED);

    REQUIRE(intfw.IsInstanceGood());
  }

  SECTION("Combined flags") {
    INotifyWrapper intfw(IN_MODIFY | IN_IGNORED);

    REQUIRE(intfw.IsInstanceGood());
  }
}

TEST_CASE("AddWatch_NonexistentFile_Fails", "[INotifyWrapped]") {
  INotifyWrapper intfw;
  REQUIRE_THROWS(intfw.AddWatch("nonexistentfile.test"));
}

TEST_CASE("AddWatch_TestFile_Succeeds", "[INotifyWrapped]") {
  SECTION("Using fallback flags") {
    INotifyWrapper intfw(IN_IGNORED | IN_MODIFY);
    int fd = -1;
    REQUIRE_NOTHROW(fd = intfw.AddWatch("/dev/null"));
    intfw.RemoveWatchByFd(fd);
  }

  SECTION("Using explicit flags") {
    INotifyWrapper intfw;
    int fd = -1;
    REQUIRE_NOTHROW(fd = intfw.AddWatch("/dev/null", IN_IGNORED | IN_MODIFY));
    intfw.RemoveWatchByFd(fd);
  }
}

TEST_CASE("RemoveWatch_TestFile_Succeeds", "[INotifyWrapped]") {
  SECTION("Using fallback flags") {
    INotifyWrapper intfw(IN_IGNORED | IN_MODIFY);
    int fd = -1;
    fd = intfw.AddWatch("/dev/null");
    REQUIRE_NOTHROW(intfw.RemoveWatchByFd(fd));
  }

  SECTION("Using explicit flags") {
    INotifyWrapper intfw;
    int fd = -1;
    fd = intfw.AddWatch("/dev/null", IN_IGNORED | IN_MODIFY);
    REQUIRE_NOTHROW(intfw.RemoveWatchByFd(fd));
  }
}
