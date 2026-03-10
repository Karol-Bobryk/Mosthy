#ifndef PROCESS_MANAGER
#define PROCESS_MANAGER

#include <string>
#include <vector>

class ProcessManager {
  std::vector<std::string> childArguments;

  pid_t processId;

public:
  ~ProcessManager();
  ProcessManager(const std::string &cmd);

  void SetCommand(const std::string &cmd);
  [[nodiscard]] bool IsProcessRunning() const;
  void StartProcess();
  void KillProcess();
};

#endif
