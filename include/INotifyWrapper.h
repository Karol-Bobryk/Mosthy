#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <string>
#include <sys/inotify.h>
#include <vector>

class INotifyWrapper {
  // File Descriptors
  int INotifyInstance;
  std::vector<int> filesSupervised;

public:
  ~INotifyWrapper();
  INotifyWrapper();

  bool IsInstanceGood();
  void AddWatch(std::string, uint32_t);
  void RemoveWatchByIndex(size_t);
  void WatchFiles(std::string);
};

#endif
