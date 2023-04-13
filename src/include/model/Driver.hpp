#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <memory>

#include "MovingObject.hpp"

namespace trafficsimulation::model
{

class Road;
class Vehicle;

/*!
 * \brief The Driver class
 *
 * Inherits and extends MovingObject abstract class.
 * Calculates driver behavior in simulation. Moves on Road objects.
 * Vehicle is friend to Driver class.
 */
class Driver : public MovingObject
{
public:
    /*!
     * \brief Constructor
     * \param std::shared_ptr<Road> road
     * \param std::unique_ptr<Vehicle> vehicle
     * \param float accelerationRate
     * \param uint32_t minDistanceToVehicleAhead
     * \param uint32_t maxSpeedOverLimit
     * \param float roadConditionSpeedModifier
     */
    Driver(const std::shared_ptr<Road> road, std::unique_ptr<Vehicle> vehicle,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier);
    virtual ~Driver();

private:
    /*!
     * \brief move
     *
     * Overrides MovingObject::move()
     * Calculates distanceTravelled_ and position_
     */
    void move() override;
    /*!
     * \brief calculateNewSpeed
     *
     * Calculates new speed of driver considering his distance to closes object in front
     */
    void calculateNewSpeed();
    /*!
     * \brief accelerate
     *
     * Accelerates vehicle speed using accelerationRate and road speedLimit
     */
    void accelerate();
    /*!
     * \brief decelerate
     * \param uint32_t tooCloseDistance
     * \param uint32_t speedDeterminer
     *
     * Decelerates vehicle speed using tooCloseDistance and road speedDeterminer
     */
    void decelerate(const uint32_t tooCloseDistance, const uint32_t speedDeterminer);
    /*!
     * \brief doStep
     * \param uint32_t step
     *
     * Increases distanceTravelled_ of Driver and vehicle_ by step or if Junction was
     * reached enters it or stops at the end of the road_
     */
    void doStep(uint32_t step);
    /*!
     * \brief selectNewPath
     *
     * Randomly selects Road from outgoing roads from Juction that is at the end of
     * road_ or if destination is set selects fastest route
     */
    void selectNewPath();

    std::shared_ptr<Road> road_;
    const std::unique_ptr<Vehicle> vehicle_;
    const float accelerationRate_; /* 0.2 - 1 */
    const uint32_t minDistanceToVehicleAhead_; /* 1000 - 8000 */
    const uint32_t maxSpeedOverLimit_; /* 0 - 30 */
    const float roadConditionSpeedModifier_;
};

} // trafficsimulation::model

#endif // DRIVER_HPP
