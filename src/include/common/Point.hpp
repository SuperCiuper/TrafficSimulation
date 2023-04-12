#ifndef POINT_HPP
#define POINT_HPP

#include<cstdint>

namespace trafficsimulation::common
{

/*!
 * \brief The Point class
 * Struct with coordinates x, y of a point
 */
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
