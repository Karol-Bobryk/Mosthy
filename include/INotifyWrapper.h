#ifndef INOTIFYWRAPPER
#define INOTIFYWRAPPER

#include <exception>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>
#include <vector>

class INotifyWrapper {
  // File Descriptor
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
  class INotifyInstanceFailure : std::exception {
    const char *what() const noexcept override {
      return "INotifyInstance failed"; // Terrible, non descriptive change it
                                       // later or smth
    }
  };
};

#endif
