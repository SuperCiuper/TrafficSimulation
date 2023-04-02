#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <memory>

#include "MovingObject.hpp"

namespace trafficsimulation::model
{

class Road;
class Vehicle;

class Driver : public MovingObject
{
public:
    Driver(const std::shared_ptr<Road> road, std::unique_ptr<Vehicle> vehicle,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier);
    virtual ~Driver();

    void move() override;

private:
    void calculateNewSpeed();
    void accelerate();
    void decelerate(const uint32_t tooCloseDistance, const uint32_t speedDeterminer);
    void doStep(uint32_t step);
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
