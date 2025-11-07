#include "INotifyWrapper.h"
#include "ProcessManager.h"
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <stdexcept>
#include <sys/fcntl.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

INotifyWrapper::~INotifyWrapper() {
  if (IsInstanceGood()) {
    close(INotifyInstance);
  }
}

bool INotifyWrapper::IsInstanceGood() { return (INotifyInstance >= 0); }

void INotifyWrapper::AddWatch(std::string path, uint32_t mask) {
  int fd = inotify_add_watch(INotifyInstance, path.c_str(),
                             mask); // TODO error checking
  filesSupervised.push_back(fd);
}

void INotifyWrapper::RemoveWatchByIndex(size_t index) {
  if (index >= filesSupervised.size())
    throw std::out_of_range("No file under given index");

  // Checking if given fd is still open
  if (fcntl(filesSupervised[index], F_GETFD) || errno == EBADF)
    inotify_rm_watch(INotifyInstance,
                     filesSupervised[index]); // TODO error checking

  filesSupervised.erase(filesSupervised.begin() + index);
}

void INotifyWrapper::WatchFiles(std::string cmd) {

  inotify_event ieStruct;

  ProcessManager pManager(cmd);

  while (true) {
    pManager.StartProcess();

    int watchStatus = read(INotifyInstance, &ieStruct, sizeof(inotify_event));

    if (watchStatus == -1)
      throw std::runtime_error("Failed to watch files");

    switch (ieStruct.mask) {
    case IN_MODIFY: {
      pManager.KillProcess();
      break;
    }

    case IN_IGNORED:
      break; // TODO: act on file removed or inode id changed
    default:
      break;
    }
  }

  RemoveWatchByIndex(0);
}

const char *INotifyWrapper::INotifyInstanceFailure::what() const noexcept {
  return "INotifyInstance failed"; // Terrible, non descriptive change it
                                   // later or smth
}
