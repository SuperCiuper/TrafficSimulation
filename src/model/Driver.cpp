#include "../include/model/Driver.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "../include/model/Junction.hpp"
#include "../include/model/Road.hpp"
#include "../include/model/Vehicle.hpp"

namespace trafficsimulation::model
{

constexpr auto SPEED_TO_DISTANCE_MODIFIER = uint32_t{10};

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
    if(vehicle_->vehicleAhead_ == nullptr)
    {
        auto distanceToJunction = road_->getLength() - distanceTravelled_;
        auto speedDeterminer = vehicle_->speed_ * vehicle_->speed_ * SPEED_TO_DISTANCE_MODIFIER
            / vehicle_->maxDeceleration_;

        if(vehicle_->speed_ == 0 && distanceToJunction != 0)
        {
            accelerate();
            vehicle_->speed_ = std::min(vehicle_->speed_, distanceToJunction);
        }

        if(distanceToJunction > speedDeterminer * 2)
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
        if(distanceToJunction < speedDeterminer)
        {
            decelerate(speedDeterminer - distanceToJunction, speedDeterminer);
        }
        return;
    }
    auto distanceToVehicle = vehicle_->vehicleAhead_->distanceTravelled_
        - vehicle_->distanceTravelled_;

    if(vehicle_->vehicleAhead_->speed_ > vehicle_->speed_
       && distanceToVehicle > minDistanceToVehicleAhead_ * 2)
    {
        accelerate();
        return;
    }
    auto speedDeterminer = (vehicle_->speed_ - vehicle_->vehicleAhead_->speed_ + 1)
         * vehicle_->speed_ * SPEED_TO_DISTANCE_MODIFIER / vehicle_->maxDeceleration_;

    if(distanceToVehicle > speedDeterminer * 3 + minDistanceToVehicleAhead_)
    {
        accelerate();
        return;
    }
    if(distanceToVehicle < speedDeterminer * 2 + minDistanceToVehicleAhead_)
    {
        decelerate(speedDeterminer * 2 + minDistanceToVehicleAhead_ - distanceToVehicle, speedDeterminer);
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
    if(speedDeterminer == 0)
    {
        vehicle_->speed_ -= std::min(vehicle_->speed_, std::min(tooCloseDistance, vehicle_->maxDeceleration_));
        return;
    }
    vehicle_->speed_ -= std::min(vehicle_->speed_, (tooCloseDistance * vehicle_->maxDeceleration_
        + (speedDeterminer - 1)) / speedDeterminer);
}

void Driver::doStep(uint32_t step)
{
    if(road_->getLength() >= distanceTravelled_ + step)
    {
        distanceTravelled_ += step;
    }
    else if(road_->getJunction()->isGreenLight(road_->getPathId()))
    {
        step = distanceTravelled_ + step - road_->getLength();
        position_ = road_->calculateNewPosition(road_->getLength());

        selectNewPath();
        doStep(step);
    }
    else
    {
        distanceTravelled_ = road_->getLength();
    }
    vehicle_->distanceTravelled_ = distanceTravelled_;
}

void Driver::selectNewPath()
{
    distanceTravelled_ = 0;
    road_->removeVehicle(vehicle_.get());
    if(vehicle_->vehicleBehind_ != nullptr)
    {
        vehicle_->vehicleBehind_->vehicleAhead_ = nullptr;
        vehicle_->vehicleAhead_ = nullptr;
    }

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
        newRoad = roads[std::rand() % std::size(roads)].lock();
    }

    if(newRoad->getStartPoint() == position_)
    {
        road_ = newRoad;
    }
    else
    {
        road_ = junction->getJunctionRoad(road_->getPathId(), newRoad->getPathId());
    }
    road_->addVehicle(vehicle_.get());
}

} // trafficsimulation::model





