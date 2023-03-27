#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <memory>

class Driver;

class Vehicle
{
public:
    Vehicle();
    Vehicle(const uint32_t maxAcceleration, const uint32_t maxDeceleration);
    ~Vehicle();

    void setVehicleAhead(const std::shared_ptr<Vehicle> vehicleAhead);
    void setVehicleBehind(const std::shared_ptr<Vehicle> vehicleBehind);

private:
    const uint32_t maxAcceleration_;
    const uint32_t maxDeceleration_;
    uint32_t speed_;
    uint32_t distanceTravelled;
    std::shared_ptr<Vehicle> vehicleAhead_;
    std::shared_ptr<Vehicle> vehicleBehind_;

    friend class Driver;
};

#endif // VEHICLE_HPP
