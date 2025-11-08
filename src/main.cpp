#include "INotifyWrapper.h"
#include <cstdlib>
#include <sys/inotify.h>

int main(int argc, char *argv[]) {

  INotifyWrapper intfw;

  intfw.AddWatch(argv[2], IN_MODIFY | IN_IGNORED);
  intfw.WatchFiles(argv[1]);

  return EXIT_SUCCESS;
}
