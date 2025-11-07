#include "ProcessManager.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
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

void ProcessManager::KillProcess() {

  kill(processId, SIGTERM);
  int status = 0;
  if (waitpid(processId, &status, 0) == -1) {
    throw std::runtime_error("Couldnt wait for child process.");
  }
}

void ProcessManager::SetCommand(std::string cmd) {

  std::istringstream ofsParser(cmd);

  for (std::string v; ofsParser >> v;) {
    childArguments.push_back(v);
  }
}
