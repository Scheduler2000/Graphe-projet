#include <stdio.h>
#include <stdlib.h>
#include "../inc/ConsoleExtension.hpp"

void ConsoleExtension::ClearConsole()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    printf("\033[H\033[J");
#endif
}

void ConsoleExtension::ClearConsoleLine(int line, int column)
{
    int currentLine = ConsoleExtension::GetCursorPosition('x');
    int currentColumn = ConsoleExtension::GetCursorPosition('y');
    ConsoleExtension::SetConsoleCursorPosition(line, column);
    printf("                                                                                                                          ");
    ConsoleExtension::SetConsoleCursorPosition(currentLine, currentColumn);
}

void ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor color)
{
    WORD col = static_cast<WORD>(color);
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
#elif __linux__
    switch (col)
    {
    case 0:
        printf(BLACK);
        break;
    case 4:
        printf(RED);
        break;
    case 2:
        printf(GREEN);
        break;
    case 14:
        printf(YELLOW);
        break;
    case 3:
        printf(BLUE);
        break;
    case 5:
        printf(PURPLE);
        break;
    case 8:
        printf(GREY);
        break;
    case 15:
        printf(DEFAULT_COLOR);
        break;
    default:
        printf(DEFAULT_COLOR);
        break;
    }
#else
#error not defined for this platform.
#endif
}

void ConsoleExtension::SetConsoleCursorPosition(int line, int column)
{
#ifdef _WIN32
    COORD position;
    position.X = line;
    position.Y = column;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
#elif __linux__
    printf("\033[%d;%dH", (line), (column));
#endif
}

int ConsoleExtension::GetCursorPosition(char component)
{
#ifdef _WIN32
    int pos = 0;

    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo);
    switch (component)
    {
    case 'x':
        return screenInfo.dwCursorPosition.X;
    case 'y':
        return screenInfo.dwCursorPosition.Y;
    }
#elif __linux__
    int x;
    int y;

    GetConsoleCursorPosition_linux(&y, &x);
    switch (component)
    {
    case 'x':
        return x;
    case 'y':
        return y;
        break;
    }
#endif
    return pos;
}

void ConsoleExtension::GetConsoleCursorPosition_linux(int *y, int *x)
{
    printf("\033[6n");
    scanf("\033[%d;%dR", x, y);
}