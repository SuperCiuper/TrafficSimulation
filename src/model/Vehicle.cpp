#include "../include/model/Vehicle.hpp"

#include <iostream>

namespace trafficsimulation::model
{

Vehicle::Vehicle(const uint32_t maxAcceleration, const uint32_t maxDeceleration)
    : maxAcceleration_{maxAcceleration}
    , maxDeceleration_{maxDeceleration}
    , speed_{0}
    , distanceTravelled_{0}
    , vehicleAhead_{}
    , vehicleBehind_{}
{
    std::cout<<" Vehicle: ";
    if(vehicleAhead_.expired()) std::cout << "expired";
    std::cout << std::endl;
}

Vehicle::~Vehicle() = default;

void Vehicle::setVehicleAhead(const std::shared_ptr<Vehicle> vehicleAhead)
{
    vehicleAhead_ = vehicleAhead;
}

void Vehicle::setVehicleBehind(const std::shared_ptr<Vehicle> vehicleBehind)
{
    vehicleBehind_ = vehicleBehind;
}

} // trafficsimulation::model
