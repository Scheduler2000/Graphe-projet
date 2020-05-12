#if !defined(__LOGGER__)
#define __LOGGER__

#include <string>

enum class LoggerPriority : short
{
    Fatal_Error,
    Error,
    Warn,
    Info,
    Debug
};

class Logger
{
private:
    LoggerPriority m_priority;

public:
    Logger(LoggerPriority priority);
    ~Logger();
    void Log(std::string const &message);
};

#endif // __LOGGER__
