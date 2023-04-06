#include "../include/model/Road.hpp"

#include "../include/model/Vehicle.hpp"

namespace trafficsimulation::model
{

Road::Road(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
    const common::Point endPoint, const std::shared_ptr<Junction> endJunction,
    const RoadCondition roadCondition, const uint32_t speedLimit)
    : Path{pathId, length, startPoint, endPoint, endJunction}
    , roadCondition_{roadCondition}
    , speedLimit_{speedLimit}
    , lastVehicle_{nullptr}
{
}

Road::~Road() = default;

RoadCondition Road::getRoadCondition() const
{
    return roadCondition_;
}

uint32_t Road::getSpeedLimit() const
{
    return speedLimit_;
}

Vehicle* Road::getLastVehicle() const
{
    return lastVehicle_;
}

void Road::addVehicle(Vehicle* const newLastVehicle)
{
    if(lastVehicle_)
    {
        lastVehicle_->setVehicleBehind(newLastVehicle);
    }
    newLastVehicle->setVehicleAhead(lastVehicle_);
    lastVehicle_ = newLastVehicle;
}

} // trafficsimulation::model
