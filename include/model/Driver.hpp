#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <memory>

#include "MovingObject.hpp"
#include "Road.hpp"
#include "Vehicle.hpp"

class Driver : public MovingObject
{
public:
    Driver(const std::shared_ptr<Road> road, const std::unique_ptr<Vehicle> vehicle);
    Driver(const std::shared_ptr<Road> road, const std::unique_ptr<Vehicle> vehicle,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit);
    virtual ~Driver();

    void move() override;

private:
    void selectNewPath() override;
    void calculateNewSpeed();

    std::shared_ptr<Road> road_;
    const std::unique_ptr<Vehicle> vehicle_;
    const uint32_t accelerationRate_;
    const uint32_t minDistanceToVehicleAhead_;
    const uint32_t maxSpeedOverLimit_;
};

#endif // DRIVER_HPP
