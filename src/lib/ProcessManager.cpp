#include "ProcessManager.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <system_error>
#include <vector>

ProcessManager::~ProcessManager() { KillProcess(); }

ProcessManager::ProcessManager(std::string command) { SetCommand(command); }

void ProcessManager::StartProcess() {

  std::vector<char *> argV;
  for (const auto &arg : childArguments) {
    argV.push_back(const_cast<char *>(arg.c_str()));
  }

  argV.push_back(nullptr);

  int status =
      posix_spawnp(&processId, argV[0], nullptr, nullptr, argV.data(), environ);

  if (status != 0) {
    std::ostringstream errOss;
    errOss << "Failed to execute the given program. ";
    errOss << strerror(errno);
    throw std::runtime_error(errOss.str());
  }
}

bool ProcessManager::IsProcessRunning() {
  int status; // TODO: add status checking
  int pid = waitpid(processId, &status, WNOHANG);
  return pid == 0;
}

void ProcessManager::KillProcess() {

  if (kill(processId, SIGTERM) == -1) {
    std::cerr << strerror(errno) << processId;
    for (auto &e : childArguments)
      std::cerr << e << "\n";
    throw std::system_error();
  }

  int status = 0; // TODO: add status checking
  if (waitpid(processId, &status, 0) == -1) {
    throw std::system_error();
  }
}

void ProcessManager::SetCommand(std::string cmd) {

  std::istringstream ofsParser(cmd);

  for (std::string v; ofsParser >> v;) {
    childArguments.push_back(v);
  }
}
