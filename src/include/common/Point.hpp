#ifndef POINT_HPP
#define POINT_HPP

#include<cstdint>

namespace trafficsimulation::common
{

struct Point
{
    int16_t x;
    int16_t y;
};

inline bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

} // trafficsimulation::common

#endif // POINT_HPP
