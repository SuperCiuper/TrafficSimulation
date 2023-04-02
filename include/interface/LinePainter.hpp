#ifndef LINEPAINTER_HPP
#define LINEPAINTER_HPP

#include "../model/Point.hpp"

namespace trafficsimulation::interface
{

class LinePainter
{
public:
    virtual void draw(const model::Point startPoint, const model::Point endPoint) = 0;

protected:
    LinePainter() = default;
    virtual ~LinePainter() = default;
};

} // trafficsimulation::interface

#endif // LINEPAINTER_HPP
