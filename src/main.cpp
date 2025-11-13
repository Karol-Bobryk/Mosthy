#include "ArgumentHandler.h"
#include "INotifyWrapper.h"
#include <cstdlib>
#include <sstream>
#include <sys/inotify.h>

int main(int argc, char *argv[]) {

  ArgumentHandler argHandler(argc, argv);

  INotifyWrapper intfw;

  auto FileArgHandler = [&intfw](std::vector<std::string> args) {
    for (const auto &arg : args)
      intfw.AddWatch(arg, IN_MODIFY | IN_IGNORED);
  };

  auto CommandArgHandler = [&intfw](std::vector<std::string> args) {
    std::ostringstream ostr;

    for (const auto &arg : args)
      ostr << arg << " ";

    intfw.WatchFiles(ostr.str());
  };

  argHandler.AddHandler("-f", FileArgHandler);
  argHandler.AddHandler("-c", CommandArgHandler);
  argHandler.RunHandler("-f");
  argHandler.RunHandler("-c");

  return EXIT_SUCCESS;
}
