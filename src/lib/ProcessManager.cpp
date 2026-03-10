#include "ProcessManager.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <vector>

ProcessManager::~ProcessManager() { KillProcess(); }

ProcessManager::ProcessManager(const std::string &cmd) : processId(0) {
  SetCommand(cmd);
}

void ProcessManager::StartProcess() {

  std::vector<char *> argV;

  argV.reserve(childArguments.size());
  for (const auto &arg : childArguments) {
    // Const cast is unfortunately required here
    argV.push_back(const_cast<char *>(arg.c_str())); // NOLINT
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

bool ProcessManager::IsProcessRunning() const {
  int status = 0; // TODO: add status checking
  int pid = waitpid(processId, &status, WNOHANG);
  return pid == 0;
}

void ProcessManager::KillProcess() {

  if (kill(processId, SIGTERM) == -1) {
    std::cerr << strerror(errno) << processId;
    for (auto &e : childArguments)
      std::cerr << e << "\n";
    // TODO: add logging as you cannot really throw here
  }

  int status = 0; // TODO: add status checking
  if (waitpid(processId, &status, 0) == -1) {
    // same here
  }
}

void ProcessManager::SetCommand(const std::string &cmd) {

  std::istringstream ofsParser(cmd);

  for (std::string v; ofsParser >> v;) {
    childArguments.push_back(v);
  }
}
