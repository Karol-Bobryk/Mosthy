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
#include <system_error>
#include <unistd.h>

INotifyWrapper::~INotifyWrapper() {
  if (IsInstanceGood()) {
    close(INotifyInstance);
  }
}

INotifyWrapper::INotifyWrapper() : INotifyInstance{inotify_init()} {
  if (!IsInstanceGood()) {
    throw std::system_error();
  }
}

bool INotifyWrapper::IsInstanceGood() { return (INotifyInstance >= 0); }

void INotifyWrapper::AddWatch(std::string path, uint32_t mask) {
  int fd = inotify_add_watch(INotifyInstance, path.c_str(), mask);
  if (fd == -1)
    throw std::system_error();

  FdToPathMap.insert({fd, path});
}

void INotifyWrapper::RemoveWatchByFd(int fd) {
  auto iter = FdToPathMap.find(fd);

  if (iter == FdToPathMap.end())
    throw std::out_of_range("Given fd is not currently supervised");

  // Checking if given fd is still open
  if (fcntl(fd, F_GETFD) || errno == EBADF) {
    int status = inotify_rm_watch(INotifyInstance, fd);
    if (status == -1)
      throw std::system_error();
  }

  FdToPathMap.erase(iter);
}

void INotifyWrapper::WatchFiles(std::string cmd) {

  inotify_event ieStruct;

  ProcessManager pManager(cmd);

  while (true) {
    pManager.StartProcess();

    int watchStatus = read(INotifyInstance, &ieStruct, sizeof(inotify_event));

    if (watchStatus == -1)
      throw std::system_error();

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

  RemoveWatchByFd(0);
}
