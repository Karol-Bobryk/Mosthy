#include "INotifyWrapper.h"
#include "Logger/LoggerService.h"
#include "ProcessManager.h"
#include <cerrno>
#include <cstring>
#include <ostream>
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
    close(iNotifyInstance);
  }
}

INotifyWrapper::INotifyWrapper(uint32_t FallbackFlags)
    : iNotifyInstance{inotify_init()}, fallbackFlags(FallbackFlags) {
  if (!IsInstanceGood()) {

    SetLogStream([](std::ostream &stream) {
      stream << "INotifyWrapper instance malfunctioned during creation "
             << strerror(errno);
    });

    LoggerService::Log(this, LogSeverity::CRITICAL);

    throw std::system_error();
  }
}

int INotifyWrapper::AddWatch(const std::string &path) {
  return AddWatch(path, fallbackFlags);
}

int INotifyWrapper::AddWatch(const std::string &path, uint32_t mask) {
  int fd = inotify_add_watch(iNotifyInstance, path.c_str(), mask);

  if (fd == -1) {
    SetLogStream([path](std::ostream &stream) {
      stream << "failed to add watch for " << path.c_str();
    });

    LoggerService::Log(this, LogSeverity::WARNING);

    throw std::system_error();
  }

  fdToPathMap.insert({fd, path});
  return fd;
}

void INotifyWrapper::RemoveWatchByFd(int fd) {
  auto iter = fdToPathMap.find(fd);

  if (iter == fdToPathMap.end()) {
    SetLogStream([&fd](std::ostream &stream) {
      stream << "Couldn't reach file with fd: " << fd;
    });

    LoggerService::Log(this, LogSeverity::INFO);
    throw std::out_of_range("Given fd is not currently supervised");
  }

  // Checking if given fd is still open
  if ((fcntl(fd, F_GETFD) != 0) || errno == EBADF) {
    int status = inotify_rm_watch(iNotifyInstance, fd);

    if (status == -1) {
      SetLogStream([&](std::ostream &stream) {
        stream << "Couldn't remove file from watch " << fdToPathMap.at(fd)
               << " " << strerror(errno);
      });

      LoggerService::Log(this, LogSeverity::WARNING);
      throw std::system_error();
    }
  }

  fdToPathMap.erase(iter);
}

void INotifyWrapper::WatchFiles(const std::string &cmd) {
  inotify_event ieStruct{};

  ProcessManager pManager(cmd);

  while (true) {
    pManager.StartProcess();

    ssize_t watchStatus =
        read(iNotifyInstance, &ieStruct, sizeof(inotify_event));

    if (watchStatus == -1) {

      SetLogStream([](std::ostream &stream) {
        stream << "watch failed " << strerror(errno);
      });

      LoggerService::Log(this, LogSeverity::WARNING);
      throw std::system_error();
    }
    switch (ieStruct.mask) {
    case IN_MODIFY: {
      pManager.KillProcess();
      break;
    }
    case IN_IGNORED:
      AddWatch(fdToPathMap.at(ieStruct.wd));
      pManager.KillProcess();
      break;
    default:
      break;
    }
  }
}

void INotifyWrapper::SetLogStream(std::function<void(std::ostream &)> func) {
  std::ostream &ostr = this->GetLogStream();
  ostr << " Fallback flags: " << this->fallbackFlags;

  func(ostr);
}
