#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <memory>

namespace trafficsimulation::model
{

class Driver;

class Vehicle
{
public:
    Vehicle(const uint32_t maxAcceleration, const uint32_t maxDeceleration);
    ~Vehicle();

    void setVehicleAhead(const std::shared_ptr<Vehicle> vehicleAhead);
    void setVehicleBehind(const std::shared_ptr<Vehicle> vehicleBehind);

private:
    const uint32_t maxAcceleration_; /* 5 - 8 */
    const uint32_t maxDeceleration_; /* 14 - 24 */
    uint32_t speed_;
    uint32_t distanceTravelled_;
    std::weak_ptr<Vehicle> vehicleAhead_;
    std::weak_ptr<Vehicle> vehicleBehind_;

    friend class Driver;
};

} // trafficsimulation::model

#endif // VEHICLE_HPP
