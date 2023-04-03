#ifndef POINTPAINTER_HPP
#define POINTPAINTER_HPP

#include "../model/Point.hpp"

namespace trafficsimulation::interface
{

class PointPainter
{
public:
    virtual ~PointPainter() = default;

    virtual void setPoint(const model::Point point, const bool highlight = false) = 0;
    virtual void paint() = 0;

protected:
    PointPainter() = default;
};

} // trafficsimulation::interface

#endif // POINTPAINTER_HPP
