#include "../include/model/Driver.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "../include/model/Junction.hpp"
#include "../include/model/Road.hpp"
#include "../include/model/Vehicle.hpp"

namespace trafficsimulation::model
{

Driver::Driver(const std::shared_ptr<Road> road, std::unique_ptr<Vehicle> vehicle,
    const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
    const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier)
    : MovingObject{}
    , road_{road}
    , vehicle_{std::move(vehicle)}
    , accelerationRate_{accelerationRate}
    , minDistanceToVehicleAhead_{minDistanceToVehicleAhead}
    , maxSpeedOverLimit_{maxSpeedOverLimit}
    , roadConditionSpeedModifier_{roadConditionSpeedModifier}
{
}

Driver::~Driver() = default;

void Driver::move()
{
    calculateNewSpeed();
    auto step = vehicle_->speed_;

    doStep(step);
    position_ = road_->calculateNewPosition(distanceTravelled_);
}

void Driver::calculateNewSpeed()
{
    if(!vehicle_->vehicleAhead_.expired())
    {
        auto distanceToJunction = road_->getLength() - distanceTravelled_;
        auto speedDeterminer = vehicle_->speed_ * vehicle_->speed_ / vehicle_->maxDeceleration_;

        if(distanceToJunction > speedDeterminer * 4)
        {
            accelerate();
            return;
        }
        if(road_->getJunction()->isGreenLight(road_->getPathId()))
        {
            if(vehicle_->speed_ < road_->getJunction()->getSpeedLimit())
            {
                accelerate();
                return;
            }
            if(vehicle_->speed_ > road_->getJunction()->getSpeedLimit())
            {
                decelerate(speedDeterminer, speedDeterminer);
                return;
            }
        }
        if(distanceToJunction < speedDeterminer * 3)
        {
            decelerate(speedDeterminer * 3 - distanceToJunction, speedDeterminer);
        }
        return;
    }
    auto distanceToVehicle = vehicle_->vehicleAhead_.lock()->distanceTravelled_
        - vehicle_->distanceTravelled_ - minDistanceToVehicleAhead_;

    auto speedDeterminer = (vehicle_->speed_ - vehicle_->vehicleAhead_.lock()->speed_)
        * vehicle_->speed_ / vehicle_->maxDeceleration_;

    if(distanceToVehicle > speedDeterminer * 4)
    {
        accelerate();
        return;
    }
    if(distanceToVehicle < speedDeterminer * 3)
    {
        decelerate(speedDeterminer * 3 - distanceToVehicle, speedDeterminer);
    }
}

void Driver::accelerate()
{
    auto maxSpeed = road_->getSpeedLimit() + maxSpeedOverLimit_;

    switch(road_->getRoadCondition())
    {
        case RoadCondition::New:
            maxSpeed *= (1 + roadConditionSpeedModifier_);
            break;
        case RoadCondition::NoPotHoles:
            break;
        case RoadCondition::SomePotHoles:
            maxSpeed *= (1 - roadConditionSpeedModifier_);
            break;
        case RoadCondition::LotsOfPotHoles:
            maxSpeed *= (1 - 2 * roadConditionSpeedModifier_);
            break;
        case RoadCondition::Offroad:
            maxSpeed *= (1 - 3 * roadConditionSpeedModifier_);
            break;
    }

    auto accelerationModifier = vehicle_->speed_ == float{1.0} ? float{1.0} :
        float{1.0} - ((float{1.0} - accelerationRate_) *
        ((static_cast<float>(vehicle_->speed_) / static_cast<float>(maxSpeed))));
    vehicle_->speed_ = std::min(maxSpeed, static_cast<uint32_t>(vehicle_->speed_ +
        std::max(uint32_t{1}, static_cast<uint32_t>(vehicle_->maxAcceleration_ * accelerationModifier))));
}

void Driver::decelerate(const uint32_t tooCloseDistance, const uint32_t speedDeterminer)
{
    vehicle_->speed_ -= (tooCloseDistance * vehicle_->maxDeceleration_ + (speedDeterminer - 1))
        / speedDeterminer;
}

void Driver::doStep(uint32_t step)
{
    if(road_->getLength() >= distanceTravelled_ + step)
    {
        distanceTravelled_ += step;
        return;
    }
    if(road_->getJunction()->isGreenLight(road_->getPathId()))
    {
        step = distanceTravelled_ + step - road_->getLength();
        position_ = road_->calculateNewPosition(road_->getLength());

        selectNewPath();
        doStep(step);
        return;
    }
    distanceTravelled_ = road_->getLength();
}

void Driver::selectNewPath()
{
    distanceTravelled_ = 0;
    vehicle_->vehicleBehind_.reset();
    const auto junction = road_->getJunction();

    if(destinationId_ == junction->getId())
    {
        destinationId_ = NO_DESTINATION;
    }

    auto newRoad = std::shared_ptr<Road>{};
    if(destinationId_ != NO_DESTINATION)
    {
        newRoad = junction->getFastestRoad(destinationId_);
    }
    else
    {
        auto roads = junction->getOutgoingRoads();
        newRoad = roads[std::rand() % std::size(roads)];
    }

    if(newRoad->getStartPoint() == position_)
    {
        road_ = newRoad;
        return;
    }
    road_ = junction->createTemporaryRoad(
        road_->calculateNewPosition(road_->getLength()), newRoad);
}

} // trafficsimulation::model





