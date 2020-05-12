#include "Point.hpp"

Point::Point(int x, int y) : m_posX(x), m_posY(y) {}

int Point::GetX() const { return m_posX; }

int Point::GetY() const { return m_posY; }

std::string Point::ToString() const { return " (" + std::to_string(m_posX) + ", " + std::to_string(m_posY) + ")"; }