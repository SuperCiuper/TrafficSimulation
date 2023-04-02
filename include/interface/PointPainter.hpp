#ifndef POINTPAINTER_HPP
#define POINTPAINTER_HPP

#include "../model/Point.hpp"

namespace trafficsimulation::interface
{

class PointPainter
{
public:
    virtual void draw(const model::Point position) = 0;

protected:
    PointPainter() = default;
    virtual ~PointPainter() = default;
};

} // trafficsimulation::interface

#endif // POINTPAINTER_HPP
