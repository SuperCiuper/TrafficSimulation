#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include <memory>
#include <string>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class PointPainter; }

namespace trafficsimulation::model
{

constexpr uint32_t NO_DESTINATION = 0x0000;

/*!
 * \brief The MovingObject abstract class
 *
 * Base class for objects that move in simulation
 */
class MovingObject
{
public:
    virtual ~MovingObject();

    void setDestination(const uint32_t newDestinationId);

    void setPainter(interface::PointPainter* const painter);
    /*!
     * \brief update
     *
     * Calls move() function.
     * If painter is set sets new position and paints object.
     * Should be called on each takt of simulation
     */
    void update();

protected:
    MovingObject();

    /*!
     * \brief move
     *
     * Pure virtual function. Calculates new distanceTravelled_ and position_
     */
    virtual void move() = 0;

    common::Point position_;
    uint32_t destinationId_;
    uint32_t distanceTravelled_;

private:
    interface::PointPainter* painter_;
};

} // trafficsimulation::model

#endif // MOVINGOBJECT_HPP
