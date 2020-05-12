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
public:
    Logger();
    ~Logger();
    static void Log(std::string const &message, LoggerPriority priority);
};

#endif
