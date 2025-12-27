#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <cstdint>
#include <map>
#include <string>
#include <sys/inotify.h>

class INotifyWrapper {
  uint32_t FallbackFlags;

  int INotifyInstance;
  std::map<int, std::string> FdToPathMap;

public:
  ~INotifyWrapper();
  INotifyWrapper(uint32_t FallbackFlags = 0);

  bool IsInstanceGood();
  int AddWatch(std::string path);
  int AddWatch(std::string path, uint32_t mask);
  void RemoveWatchByFd(int fd);
  void WatchFiles(std::string cmd);
};
#endif
