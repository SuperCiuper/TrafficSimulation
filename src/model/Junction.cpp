#include "../include/model/Junction.hpp"

#include <algorithm>
#include <cstdlib>

#include <QTimer>

#include "../include/interface/PointPainter.hpp"
#include "../include/model/Path.hpp"
#include "../include/model/Road.hpp"

#include <iostream>

namespace trafficsimulation::model
{

constexpr uint32_t ALL_RED_LIGHTS_TIME = 1000;
constexpr uint32_t DUMMY_ID = 0xFFFF;
// usual Vehicle will drive through Junction in 100 ticks - 10 sec
constexpr uint32_t TEMPORARY_PATH_LENGTH = 20000;
constexpr RoadCondition TEMPORARY_ROAD_CONDITION = RoadCondition::SomePotHoles;

Junction::Junction(const uint32_t junctionId, const common::Point position)
    : junctionId_{junctionId}
    , position_ {position}
    , speedLimit_{300}
    , incomingRoadPathIds_{}
    , roadWithGreenIterator_{0}
    , roadGreenLight_{false}
    , pavementGreenLight_{true}
    , wasLastGreenLightRoad_{false}
    , outgoingRoads_{}
    , outgoingPavements_{}
    , fastestRoutes_{}
{
    auto timeout = std::rand() % 12000 + 24000; /* timeout of 24 - 36 sec */

    auto redLightTimer = new QTimer();
    connect(redLightTimer, &QTimer::timeout, this, &Junction::changeLights);
    redLightTimer->start(timeout);

    QTimer::singleShot(2000, this, [this, timeout]() {
        auto greenLightTimer = new QTimer();
        connect(greenLightTimer, &QTimer::timeout, this, &Junction::changeLights);
        greenLightTimer->start(timeout);
    });
}

Junction::Junction(const uint32_t junctionId, const common::Point position,
    const uint32_t pathWithGreenLightId)
    : junctionId_{junctionId}
    , position_ {position}
    , speedLimit_{300}
    , incomingRoadPathIds_{}
    , roadWithGreenIterator_{0}
    , roadGreenLight_{true}
    , pavementGreenLight_{true}
    , wasLastGreenLightRoad_{false}
    , outgoingRoads_{}
    , outgoingPavements_{}
    , fastestRoutes_{}
{
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
    if (roadGreenLight_ && pathId == incomingRoadPathIds_[roadWithGreenIterator_])
    {
        return true;
    }
    if (std::find(incomingRoadPathIds_.begin(), incomingRoadPathIds_.end(), pathId)
        == std::cend(incomingRoadPathIds_))
    {
        return pavementGreenLight_;
    }
    return false;
}

std::vector<std::shared_ptr<Road>> Junction::getOutgoingRoads() const
{
    return outgoingRoads_;
}

std::vector<std::shared_ptr<Path>> Junction::getOutgoingPavements() const
{
    return outgoingPavements_;
}

const std::shared_ptr<Road> Junction::getFastestRoad(const uint32_t destinationId)
{
    if(std::size(outgoingRoads_) == 1)
    {
        return outgoingRoads_.front();
    }
    return fastestRoutes_.at(destinationId).first;
}

const std::shared_ptr<Path> Junction::getFastestPavement(const uint32_t destinationId)
{
    if(std::size(outgoingPavements_) == 1)
    {
        return outgoingPavements_.front();
    }
    return fastestRoutes_.at(destinationId).second;
}

void Junction::addIncomingRoadId(const uint32_t roadId)
{
    incomingRoadPathIds_.push_back(roadId);
    roadWithGreenIterator_ = roadId;
}

void Junction::addOutgoingRoad(const std::shared_ptr<Road> newRoad)
{
    outgoingRoads_.push_back(newRoad);
}

void Junction::addOutgoingPavement(const std::shared_ptr<Path> newPavement)
{
    outgoingPavements_.push_back(newPavement);
}

void Junction::setFastestRoute(const uint32_t destinationId,
    const std::pair<std::shared_ptr<Road>, std::shared_ptr<Path> > bestPaths)
{
    fastestRoutes_.emplace(destinationId, bestPaths);
}

std::shared_ptr<Road> Junction::createTemporaryRoad(const common::Point startPoint,
    const std::shared_ptr<Road> newRoad) const
{
    auto tempJunction = std::make_shared<Junction>(DUMMY_ID,
        newRoad->getStartPoint(), DUMMY_ID);
    tempJunction->addOutgoingRoad(newRoad);
    return std::make_shared<Road>(DUMMY_ID, TEMPORARY_PATH_LENGTH,
        startPoint, newRoad->getStartPoint(), tempJunction,
        TEMPORARY_ROAD_CONDITION, speedLimit_);
}

std::shared_ptr<Path> Junction::createTemporaryPavement(const common::Point startPoint,
    const std::shared_ptr<Path> newPavement) const
{
    auto tempJunction = std::make_shared<Junction>(DUMMY_ID,
        newPavement->getStartPoint(), DUMMY_ID);
    tempJunction->addOutgoingPavement(newPavement);
    return std::make_shared<Path>(DUMMY_ID, TEMPORARY_PATH_LENGTH,
        startPoint, newPavement->getStartPoint(), tempJunction);
}

void Junction::changeLights()
{
    if (pavementGreenLight_)
    {
        pavementGreenLight_ = false;
        wasLastGreenLightRoad_ = false;
        return;
    }
    if (roadGreenLight_)
    {
        roadGreenLight_ = false;
        wasLastGreenLightRoad_ = true;
        return;
    }
    if (!wasLastGreenLightRoad_)
    {
        const auto lenght = std::size(incomingRoadPathIds_);
        if(lenght)
        {
            roadWithGreenIterator_++;
            if(lenght == roadWithGreenIterator_)
            {
                roadWithGreenIterator_ = 0;
            }
            roadGreenLight_ = true;
            return;
        }
    }
    pavementGreenLight_ = true;
}

void Junction::setPainter(std::unique_ptr<interface::PointPainter> painter)
{
    painter_ = std::move(painter);
    painter_->setPoint(position_);
}

void Junction::update()
{
    std::cout << " junction paint " << std::endl;
    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->paint();
}

} // trafficsimulation::model
