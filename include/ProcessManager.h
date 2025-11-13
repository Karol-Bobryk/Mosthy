#ifndef PROCESS_MANAGER
#define PROCESS_MANAGER

#include <string>
#include <vector>

class ProcessManager {

  std::vector<std::string> childArguments;

  pid_t processId;

public:
  ~ProcessManager();
  ProcessManager(std::string);

  void SetCommand(std::string);

  void StartProcess();
  void KillProcess();
};

#endif
