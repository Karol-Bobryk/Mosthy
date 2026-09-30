
#ifndef LOG_SERIALIZABLE
#define LOG_SERIALIZABLE

#include <functional>
#include <sstream>

class LogSerializable {
protected:
  std::ostringstream logOutStream;

public:
  virtual ~LogSerializable() = default;
  LogSerializable() = default;

  std::string GetLogString() { return logOutStream.str(); }
  std::ostream &GetLogStream() { return logOutStream; };

  virtual void SetLogStream(std::function<void(std::ostream &)> func) = 0;
};

#endif
