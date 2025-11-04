#include "INotifyWrapper.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/inotify.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  INotifyWrapper intfw;
  intfw.AddWatch(argv[1], IN_MODIFY | IN_IGNORED);
  intfw.WatchFiles();

  return EXIT_SUCCESS;
}
