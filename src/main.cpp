#include "ArgumentHandler.h"
#include "INotifyWrapper.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <sys/inotify.h>
#include <vector>

int main(int argc, char *argv[]) {

  ArgumentHandler argHandler(argc, argv);
  INotifyWrapper intfw(IN_MODIFY | IN_IGNORED);

  auto fileArgHandler = [&intfw](const std::vector<std::string> &args) {
    for (const auto &arg : args)
      intfw.AddWatch(arg, IN_MODIFY | IN_IGNORED);
  };

  auto commandArgHandler = [&intfw](const std::vector<std::string> &args) {
    std::ostringstream ostr("");

    for (const auto &arg : args)
      ostr << arg << " ";

    intfw.WatchFiles(ostr.str());
  };

  argHandler.AddHandler("f", fileArgHandler);
  argHandler.AddHandler("c", commandArgHandler);
  argHandler.RunHandler("f");
  argHandler.RunHandler("c");

  return EXIT_SUCCESS;
}
