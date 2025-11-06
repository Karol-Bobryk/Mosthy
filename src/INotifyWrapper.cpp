#include "INotifyWrapper.h"
#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <iostream>
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
  int fd = inotify_add_watch(INotifyInstance, path.c_str(), mask);
  filesSupervised.push_back(fd);
}

void INotifyWrapper::RemoveWatchByIndex(size_t index) {
  if (index >= filesSupervised.size())
    throw std::out_of_range("No file under given index");

  // Checking if given fd is still open
  if (fcntl(filesSupervised[index], F_GETFD) || errno == EBADF)
    inotify_rm_watch(INotifyInstance, filesSupervised[index]);

  filesSupervised.erase(filesSupervised.begin() + index);
}

void INotifyWrapper::WatchFiles(std::string cmd) {

  inotify_event ieStruct;

  pid_t processId;

  while (true) {
    processId = fork();

    switch (processId) {
    case -1:
      throw std::runtime_error(
          "Program refused to fork, will add spork support later");
      break;
    case 0: {
      std::vector<std::string> childArgsStr = {"ls", "-l"};
      std::vector<char *> childArgs;

      for (const auto &arg : childArgsStr) {
        childArgs.push_back(const_cast<char *>(arg.c_str()));
      }
      childArgs.push_back(nullptr);

      int status =
          execvp(cmd.c_str(), childArgs.data()); // input not sanitized btw
      perror("");
      exit(1);
      break;
    }

    default:
      // YE OLDE PROCESS
      break;
    }

    read(INotifyInstance, &ieStruct, sizeof(inotify_event));

    switch (ieStruct.mask) {
    case IN_MODIFY: {
      std::cout << processId << std::endl;
      kill(processId, SIGTERM);

      int status = 0;
      if (waitpid(processId, &status, 0) == -1) {
        throw std::runtime_error("Couldnt wait for child process");
      }
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
