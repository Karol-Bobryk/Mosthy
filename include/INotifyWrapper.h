#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <map>
#include <string>
#include <sys/inotify.h>

class INotifyWrapper {
  // File Descriptors
  int INotifyInstance;
  std::map<int, std::string> FdToPathMap;

public:
  ~INotifyWrapper();
  INotifyWrapper();

  bool IsInstanceGood();
  void AddWatch(std::string, uint32_t);
  void RemoveWatchByFd(int);
  void WatchFiles(std::string);
};

#endif
