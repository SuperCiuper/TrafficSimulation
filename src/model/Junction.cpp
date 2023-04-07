#include "../include/model/Junction.hpp"

#include <algorithm>
#include <cstdlib>

#include <QTimer>

#include "../include/interface/PointPainter.hpp"
#include "../include/model/Path.hpp"
#include "../include/model/Road.hpp"

namespace trafficsimulation::model
{

constexpr uint32_t PEDESTRIANREDLIGHTTIME = 18000;
constexpr uint32_t DRIVERREDLIGHTTIME = 6000;
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
    , pavementGreenLight_{false}
    , wasLastGreenLightRoad_{true}
    , outgoingRoads_{}
    , outgoingPavements_{}
    , fastestRoutes_{}
{
    auto timeout = std::rand() * std::rand() % 12001 + 24000; /* timeout of 24 - 36 sec */
    auto pedestrianGreenLightTimer = new QTimer();
    connect(pedestrianGreenLightTimer, &QTimer::timeout, this, &Junction::changeLights);
    pedestrianGreenLightTimer->start(timeout * 2);

    QTimer::singleShot(timeout - PEDESTRIANREDLIGHTTIME, this, [this, timeout]() {
        auto pedestrianRedLightTimer = new QTimer();
        connect(pedestrianRedLightTimer, &QTimer::timeout, this, &Junction::changeLights);
        pedestrianRedLightTimer->start(timeout * 2);
    });

    QTimer::singleShot(timeout, this, [this, timeout]() {
        auto driverGreenLightTimer = new QTimer();
        connect(driverGreenLightTimer, &QTimer::timeout, this, &Junction::changeLights);
        driverGreenLightTimer->start(timeout * 2);
    });

    QTimer::singleShot(timeout * 2 - DRIVERREDLIGHTTIME, this, [this, timeout]() {
        auto driverRedLightTimer = new QTimer();
        connect(driverRedLightTimer, &QTimer::timeout, this, &Junction::changeLights);
        driverRedLightTimer->start(timeout * 2);
    });
}

Junction::Junction(const uint32_t junctionId, const common::Point position,
    const uint32_t pathWithGreenLightId)
    : junctionId_{junctionId}
    , position_ {position}
    , speedLimit_{300}
    , incomingRoadPathIds_{DUMMY_ID}
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

void Junction::addIncomingRoadId(const uint32_t roadId)
{
    incomingRoadPathIds_.push_back(roadId);
}

void Junction::addOutgoingRoad(const std::shared_ptr<Road> newRoad)
{
    outgoingRoads_.push_back(std::weak_ptr<Road>{newRoad});
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
        roadWithGreenIterator_++;
        if(std::size(incomingRoadPathIds_) == roadWithGreenIterator_)
        {
            roadWithGreenIterator_ = 0;
        }
        roadGreenLight_ = true;
        return;
    }
    pavementGreenLight_ = true;
}

void Junction::setPainter(interface::PointPainter* const painter)
{
    painter_ = painter;
    painter_->setPoint(position_);
}

void Junction::update()
{
    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->paint();
}

} // trafficsimulation::model
