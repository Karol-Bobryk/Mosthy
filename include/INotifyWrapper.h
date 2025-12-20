#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <cstdint>
#include <map>
#include <string>
#include <sys/inotify.h>

class INotifyWrapper {
  // File Descriptors
  uint32_t FallbackFlags;

  int INotifyInstance;
  std::map<int, std::string> FdToPathMap;

public:
  ~INotifyWrapper();
  INotifyWrapper();
  INotifyWrapper(uint32_t FallbackFlags);

  bool IsInstanceGood();
  void AddWatch(std::string path);
  void AddWatch(std::string path, uint32_t mask);
  void RemoveWatchByFd(int fd);
  void WatchFiles(std::string cmd);
};
#endif
