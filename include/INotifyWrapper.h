#ifndef INOTIFY_WRAPPER
#define INOTIFY_WRAPPER

#include "Logger/LogSerializable.h"
#include <cstdint>
#include <map>
#include <string>
#include <sys/inotify.h>

class INotifyWrapper : LogSerializable {
  uint32_t fallbackFlags;

  int iNotifyInstance;
  std::map<int, std::string> fdToPathMap;

public:
  ~INotifyWrapper() override;
  INotifyWrapper(uint32_t FallbackFlags = 0);

  [[nodiscard]] bool IsInstanceGood() const { return (iNotifyInstance >= 0); }

  int AddWatch(const std::string &path);
  int AddWatch(const std::string &path, uint32_t mask);
  void RemoveWatchByFd(int fd);
  void WatchFiles(const std::string &cmd);
  void SetLogStream(std::function<void(std::ostream &)> func) override;
};
#endif
