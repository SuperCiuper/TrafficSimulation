#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include <memory>
#include <string>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class PointPainter; }

namespace trafficsimulation::model
{

constexpr uint32_t NO_DESTINATION = 0x0000;

class MovingObject
{
public:
    virtual ~MovingObject();
    uint32_t getDestination() const;

    void setDestination(const uint32_t newDestinationId);

    void setPainter(interface::PointPainter* const painter);
    void update();

protected:
    MovingObject();

    virtual void move() = 0;

    common::Point position_;
    uint32_t destinationId_;
    uint32_t distanceTravelled_;

private:
    interface::PointPainter* painter_;
};

} // trafficsimulation::model

#endif // MOVINGOBJECT_HPP
