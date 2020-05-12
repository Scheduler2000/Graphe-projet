#if !defined(__APPLICATION_MENU__)
#define __APPLICATION_MENU__

#include <string>

class ApplicationMenu
{
private:
    void dessinerAscii() const;
    void afficherChoix() const;

public:
    void Run() const;
};

#endif // __APPLICATION_MENU__
