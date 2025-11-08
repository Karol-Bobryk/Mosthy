#include "ArgumentHandler.h"
#include "INotifyWrapper.h"
#include <cstdlib>
#include <sys/inotify.h>

int main(int argc, char *argv[]) {

  TestArgs flagHandlerArgs;
  flagHandlerArgs.test = 1;
  flagHandlerArgs.args = {"a"};
  ArgumentHandler argHandler(argc, argv);

  std::pair<std::vector<std::string>,
            std::function<void(FlagHandlerArguments &)>>
      p = std::make_pair(flagHandlerArgs.args, [](FlagHandlerArguments &args) {
        // std::cout << args.args[0] << std::endl;
      });

  argHandler.FlagToFunctionMap.insert({"-f", p});

  argHandler.FlagToFunctionMap.at("-f").second(flagHandlerArgs);

  INotifyWrapper intfw;

  intfw.AddWatch(argv[2], IN_MODIFY | IN_IGNORED);
  intfw.WatchFiles(argv[1]);

  return EXIT_SUCCESS;
}
