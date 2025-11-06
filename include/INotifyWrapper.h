#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <exception>
#include <string>
#include <sys/inotify.h>
#include <vector>

class INotifyWrapper {
  // File Descriptors
  int INotifyInstance;
  std::vector<int> filesSupervised;

public:
  ~INotifyWrapper();
  INotifyWrapper() : INotifyInstance{inotify_init()} {
    if (INotifyInstance < 0) {
      throw INotifyInstanceFailure();
    }
  }

  bool IsInstanceGood();
  void AddWatch(std::string, uint32_t);
  void RemoveWatchByIndex(size_t);
  void WatchFiles(std::string);

  class INotifyInstanceFailure : std::exception {
    const char *what() const noexcept override;
  };
};

#endif
