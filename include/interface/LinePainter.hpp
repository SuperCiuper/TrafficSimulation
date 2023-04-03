#ifndef LINEPAINTER_HPP
#define LINEPAINTER_HPP

#include "../model/Point.hpp"

namespace trafficsimulation::interface
{

class LinePainter
{
public:
    virtual ~LinePainter() = default;

    virtual void setPoints(const model::Point startPoint, const model::Point endPoint) = 0;
    virtual void paint() = 0;

protected:
    LinePainter() = default;
};

} // trafficsimulation::interface

#endif // LINEPAINTER_HPP
