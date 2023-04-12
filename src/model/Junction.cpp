#include "../include/model/Junction.hpp"

#include <algorithm>
#include <cstdlib>

#include "../include/interface/PointPainter.hpp"
#include "../include/model/Path.hpp"
#include "../include/model/Road.hpp"

namespace trafficsimulation::model
{

constexpr uint32_t PEDESTRIANREDLIGHTTICKS = 240;
constexpr uint32_t DRIVERREDLIGHTTICKS = 60;
constexpr uint32_t DUMMY_ID = 0xFFFF;
// usual Vehicle will drive through Junction in 100 ticks - 10 sec
constexpr uint32_t TEMPORARY_PATH_LENGTH = 20000;
constexpr uint32_t TEMPORARY_PATH_MAX_SPEED_LIMIT = 300;
constexpr RoadCondition TEMPORARY_ROAD_CONDITION = RoadCondition::SomePotHoles;

Junction::Junction(const uint32_t junctionId, const common::Point position)
    : junctionId_{junctionId}
    , position_ {position}
    , speedLimit_{TEMPORARY_PATH_MAX_SPEED_LIMIT}
    , incomingRoads_{}
    , lightState_{LightState::PedestrianGreenLight}
    , roadWithGreenIterator_{0}
    , timeoutTicks_{0}
    , ticksToLightChange_{0}
    , outgoingRoads_{}
    , outgoingPavements_{}
    , junctionRoads_{}
    , fastestRoutes_{}
{
    timeoutTicks_ = std::rand() * std::rand() % 191 + 260; /* of 260 - 450 ticks -> default 20 - 32 sec*/
    ticksToLightChange_ = timeoutTicks_ - PEDESTRIANREDLIGHTTICKS;
}

Junction::~Junction() = default;

uint32_t Junction::getId() const
{
    return junctionId_;
}

common::Point Junction::getPosition() const
{
    return position_;
}

uint32_t Junction::getSpeedLimit() const
{
    return speedLimit_;
}

bool Junction::isGreenLight(const uint32_t pathId) const
{
    /* is either dummy junction or only pedestrians cross it */
    if (std::size(incomingRoads_) == 0)
    {
        return true;
    }
    if (lightState_ == LightState::PedestrianGreenLight
         && std::find_if(incomingRoads_.begin(), incomingRoads_.end(),
            [pathId](const std::weak_ptr<Road> road){ return road.lock()->getPathId() == pathId; })
            == std::cend(incomingRoads_))
    {
        return true;
    }
    if (lightState_ == LightState::DriverGreenLight
         && pathId == incomingRoads_[roadWithGreenIterator_].lock()->getPathId())
    {
        return true;
    }
    return false;
}

std::vector<std::weak_ptr<Road>> Junction::getOutgoingRoads() const
{
    return outgoingRoads_;
}

std::vector<std::weak_ptr<Path>> Junction::getOutgoingPavements() const
{
    return outgoingPavements_;
}

std::shared_ptr<Road> Junction::getFastestRoad(const uint32_t destinationId)
{
    if(std::size(outgoingRoads_) == 1)
    {
        return outgoingRoads_.front().lock();
    }
    return fastestRoutes_.at(destinationId).first.lock();
}

std::shared_ptr<Path> Junction::getFastestPavement(const uint32_t destinationId)
{
    if(std::size(outgoingPavements_) == 1)
    {
        return outgoingPavements_.front().lock();
    }
    return fastestRoutes_.at(destinationId).second.lock();
}

void Junction::addIncomingRoad(const std::shared_ptr<Road> newRoad)
{
    incomingRoads_.push_back(std::weak_ptr<Road>{newRoad});

    for(const auto& outgoingRoad : outgoingRoads_)
    {
        auto lockedRoad = outgoingRoad.lock();
        auto tempJunction = std::make_shared<Junction>(DUMMY_ID,
            lockedRoad->getStartPoint());
        tempJunction->addOutgoingRoad(lockedRoad);

        junctionRoads_[newRoad->getPathId()][lockedRoad->getPathId()] =
            std::make_shared<Road>(DUMMY_ID, TEMPORARY_PATH_LENGTH,
            newRoad->calculateNewPosition(newRoad->getLength()),
            lockedRoad->getStartPoint(), tempJunction,
            TEMPORARY_ROAD_CONDITION, speedLimit_);
    }
}

void Junction::addOutgoingRoad(const std::shared_ptr<Road> newRoad)
{
    outgoingRoads_.push_back(std::weak_ptr<Road>{newRoad});

    for(const auto& incomingRoad : incomingRoads_)
    {
        auto lockedRoad = incomingRoad.lock();
        auto tempJunction = std::make_shared<Junction>(DUMMY_ID,
            newRoad->getStartPoint());
        tempJunction->addOutgoingRoad(newRoad);

        junctionRoads_[lockedRoad->getPathId()][newRoad->getPathId()] =
            std::make_shared<Road>(DUMMY_ID, TEMPORARY_PATH_LENGTH,
            lockedRoad->calculateNewPosition(lockedRoad->getLength()),
            newRoad->getStartPoint(), tempJunction,
            TEMPORARY_ROAD_CONDITION, speedLimit_);
    }
}

void Junction::addOutgoingPavement(const std::shared_ptr<Path> newPavement)
{
    outgoingPavements_.push_back(std::weak_ptr<Path>{newPavement});
}

void Junction::setFastestRoute(const uint32_t destinationId,
    const std::pair<std::weak_ptr<Road>, std::weak_ptr<Path>> bestPaths)
{
    fastestRoutes_.emplace(destinationId, bestPaths);
}

std::shared_ptr<Road> Junction::getJunctionRoad(const uint32_t oldRoadId, const uint32_t newRoadId) const
{
    return junctionRoads_.at(oldRoadId).at(newRoadId);
}

std::shared_ptr<Path> Junction::createTemporaryPavement(const common::Point startPoint,
    const std::shared_ptr<Path> newPavement) const
{
    auto tempJunction = std::make_shared<Junction>(DUMMY_ID,
        newPavement->getStartPoint());
    tempJunction->addOutgoingPavement(newPavement);
    return std::make_shared<Path>(DUMMY_ID, TEMPORARY_PATH_LENGTH,
        startPoint, newPavement->getStartPoint(), tempJunction);
}

void Junction::changeLights()
{
    switch(lightState_)
    {
        case LightState::PedestrianGreenLight:
            lightState_ = LightState::PedestrianRedLight;
            ticksToLightChange_ = PEDESTRIANREDLIGHTTICKS;
            break;
        case LightState::PedestrianRedLight:
            roadWithGreenIterator_++;
            if(std::size(incomingRoads_) == roadWithGreenIterator_)
            {
                roadWithGreenIterator_ = 0;
            }
            lightState_ = LightState::DriverGreenLight;
            ticksToLightChange_ = timeoutTicks_ - DRIVERREDLIGHTTICKS;
            break;
        case LightState::DriverGreenLight:
            lightState_ = LightState::DriverRedLight;
            ticksToLightChange_ = DRIVERREDLIGHTTICKS;
            break;
        case LightState::DriverRedLight:
            lightState_ = LightState::PedestrianGreenLight;
            ticksToLightChange_ = timeoutTicks_ - PEDESTRIANREDLIGHTTICKS;
            break;
    }
}

void Junction::setPainter(interface::PointPainter* const painter)
{
    painter_ = painter;
    painter_->setPoint(position_);
}

void Junction::update()
{
    ticksToLightChange_--;
    if(ticksToLightChange_ == 0)
    {
        changeLights();
    }

    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->paint();
}

} // trafficsimulation::model
