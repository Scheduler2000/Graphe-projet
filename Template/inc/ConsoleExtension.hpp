#ifndef __CONSOLE_EXTENSION__
#define __CONSOLE_EXTENSION__

#ifdef _WIN32
#include <windows.h>

#elif __linux__

#include <termios.h>
#include <unistd.h>
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define GREY "\033[0;37m"
#define DEFAULT_COLOR "\033[0;m"
#define Sleep(s) sleep(s)
#endif

namespace ConsoleExtension
{
enum class ConsoleColor
{
    Black = 0,
    Green = 2,
    Blue = 3,
    Red = 4,
    Purple = 5,
    Grey = 8,
    Turquoise = 11,
    LightRed = 12,
    LightPurple = 13,
    Yellow = 14,
    Default_Color = 15
};

void SetConsoleCursorPosition(int line, int column);
void SetConsoleTextColor(ConsoleColor color);

int GetCursorPosition(char component);
void GetConsoleCursorPosition_linux(int *y, int *x);

void ClearConsole();
void ClearConsoleLine(int line, int column);

} // namespace ConsoleExtension
#endif
