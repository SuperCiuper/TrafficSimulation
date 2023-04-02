#include "../../include/model/Road.hpp"

#include "../../include/model/Vehicle.hpp"

namespace trafficsimulation::model
{

Road::Road(const uint32_t pathId, const uint32_t length, const Point startPoint,
    const Point endPoint, const std::shared_ptr<Junction> endJunction,
    const RoadCondition roadCondition, const uint32_t speedLimit)
    : Path{pathId, length, startPoint, endPoint, endJunction}
    , roadCondition_{roadCondition}
    , speedLimit_{speedLimit}
    , lastVehicle_{}
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

const std::shared_ptr<Vehicle> Road::getLastVehicle() const
{
    return std::shared_ptr<Vehicle>{lastVehicle_};
}

void Road::addVehicle(std::shared_ptr<Vehicle> newLastVehicle)
{
    auto lockedVehicle = lastVehicle_.lock();
    if(lockedVehicle)
    {
        lockedVehicle->setVehicleBehind(newLastVehicle);
    }
    newLastVehicle->setVehicleAhead(lockedVehicle);
    lastVehicle_ = newLastVehicle;
}

} // trafficsimulation::model
