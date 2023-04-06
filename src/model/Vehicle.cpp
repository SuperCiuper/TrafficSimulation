#include "../include/model/Vehicle.hpp"

#include <iostream>

namespace trafficsimulation::model
{

Vehicle::Vehicle(const uint32_t maxAcceleration, const uint32_t maxDeceleration)
    : maxAcceleration_{maxAcceleration}
    , maxDeceleration_{maxDeceleration}
    , speed_{0}
    , distanceTravelled_{0}
    , vehicleAhead_{nullptr}
    , vehicleBehind_{nullptr}
{
}

Vehicle::~Vehicle() = default;

void Vehicle::setVehicleAhead(Vehicle* const vehicleAhead)
{
    vehicleAhead_ = vehicleAhead;
}

void Vehicle::setVehicleBehind(Vehicle* const vehicleBehind)
{
    vehicleBehind_ = vehicleBehind;
}

} // trafficsimulation::model
