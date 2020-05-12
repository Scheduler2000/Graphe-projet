#if !defined(__POINT__)
#define __POINT__

#include <string>

class Point
{
private:
    int m_posX;
    int m_posY;

public:
    Point(int x, int y);

    int GetX() const;
    int GetY() const;

    std::string ToString() const;
};

#endif // __POINT__
