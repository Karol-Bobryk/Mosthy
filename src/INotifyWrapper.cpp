#include <INotifyWrapper.h>

INotifyWrapper::~INotifyWrapper() {
  if (IsInstanceGood()) {
    close(INotifyInstance);
  }
}

void INotifyWrapper::AddWatch(std::string path, uint32_t mask) {
  int fd = inotify_add_watch(INotifyInstance, path.c_str(), mask);
  filesSupervised.push_back(fd);
}

bool INotifyWrapper::IsInstanceGood() { return (INotifyInstance >= 0); }
