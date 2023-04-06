#include "../include/model/Simulation.hpp"

#include <cstdlib>
#include <ctime>

#include "../include/interface/LinePainter.hpp"
#include "../include/interface/PointPainter.hpp"

#include "../include/model/Driver.hpp"
#include "../include/model/Junction.hpp"
#include "../include/model/Path.hpp"
#include "../include/model/Pedestrian.hpp"
#include "../include/model/Road.hpp"
#include "../include/model/Vehicle.hpp"

namespace trafficsimulation::model
{

constexpr auto ROADOFFSET = uint32_t{3};
constexpr auto PAVEMENTOFFSET = uint32_t{8};
constexpr auto SPAWNPATHSLENGTH = uint32_t{100000};
constexpr auto REFRESHRATE = uint32_t{100};
constexpr auto BASESTARTPOINT = common::Point{-100, -100};
constexpr auto BASEENDPOINT = common::Point{20, 20};

Simulation::Simulation()
    : junctionId_{0x1000}
    , pathId_{0x2000}
    , junctions_{}
    , drivers_{}
    , pedestrians_{}
    , roadConnections_{}
    , pavementConnections_{}
    , spawnRoad_{nullptr}
    , spawnPavement_{nullptr}
    , simulationRefreshTimer_{nullptr}
    , basePrintersSet_{false}
{
    std::srand(std::time(0));
    generateBaseSimulation();

    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Simulation::updateObjects);
    simulationRefreshTimer_ = std::unique_ptr<QTimer>(timer);
}

Simulation::~Simulation() = default;

void Simulation::setBasePrinters(interface::PointPainter* const junctionPainter,
    interface::LinePainter* const roadPainter,
    interface::LinePainter* const pavementPainter)
{
    junctions_.front()->setPainter(junctionPainter);
    spawnRoad_->setPainter(roadPainter);
    spawnPavement_->setPainter(pavementPainter);
    basePrintersSet_ = true;
}

std::optional<std::string> Simulation::start()
{
    if(!basePrintersSet_)
    {
        return "Base printers not set - program stopped!";
    }
    for(const auto& roadConnection : roadConnections_)
    {
        if(std::size(roadConnection.second) == 0)
        {
            return "At least one junction does not have any road going out of it";
        }
    }
    for(const auto& pavementConnection : pavementConnections_)
    {
        if(std::size(pavementConnection.second) == 0)
        {
            return "At least one junction does not have any pavement going out of it";
        }
    }
    if(!simulationRefreshTimer_->isActive())
    {
        simulationRefreshTimer_->start(REFRESHRATE);
        return {};
    }
    return "Unknown error";
}

void Simulation::stop()
{
    if(simulationRefreshTimer_->isActive())
    {
        simulationRefreshTimer_->stop();
    }
}

bool Simulation::isRunning()
{
    return simulationRefreshTimer_->isActive();
}

const std::vector<std::shared_ptr<Junction>> &Simulation::getJunctions() const
{
    return junctions_;
}

const std::vector<std::shared_ptr<Driver> > &Simulation::getDrivers() const
{
    return drivers_;
}

const std::vector<std::shared_ptr<Pedestrian> > &Simulation::getPedestrians() const
{
    return pedestrians_;
}

const std::map<uint32_t, std::vector<uint32_t>> Simulation::getConnectedJunctionsByRoad() const
{
    auto connectedJunctions = std::map<uint32_t, std::vector<uint32_t>>{};

    for(const auto& roads : roadConnections_)
    {
        auto startJunctionId = roads.first;

        for(const auto& road : roads.second)
        {
            connectedJunctions[startJunctionId].push_back(road->getJunction()->getId());
        }
    }
    return connectedJunctions;
}

const std::map<uint32_t, std::vector<uint32_t>> Simulation::getConnectedJunctionsByPavement() const
{
    auto connectedJunctions = std::map<uint32_t, std::vector<uint32_t>>{};

    for(const auto& pavements : pavementConnections_)
    {
        auto startJunctionId = pavements.first;

        for(const auto& road : pavements.second)
        {
            connectedJunctions[startJunctionId].push_back(road->getJunction()->getId());
        }
    }
    return connectedJunctions;
}

void Simulation::addJunction(const common::Point position,
    interface::PointPainter* const painter)
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, position);
    junction->setPainter(painter);
    junction->update();
    junctions_.push_back(junction);
    roadConnections_[junctionId_] = {};
    pavementConnections_[junctionId_] = {};
}

void Simulation::addRoad(const std::shared_ptr<Junction> startJunction,
    const std::shared_ptr<Junction> endJunction, const uint32_t length,
    const RoadCondition roadCondition, const uint32_t speedLimit,
    interface::LinePainter* const painter)
{
    pathId_++;
    auto startPoint = startJunction->getPosition();
    auto endPoint = endJunction->getPosition();
    calculatePathPoints(startPoint, endPoint, ROADOFFSET, length);

    auto road = std::make_shared<Road>(pathId_, length, startPoint, endPoint,
        endJunction, roadCondition, speedLimit);
    road->setPainter(painter);
    road->update();
    startJunction->addOutgoingRoad(road);
    endJunction->addIncomingRoadId(pathId_);
    roadConnections_[startJunction->getId()].push_back(road);
}

void Simulation::addPavement(const std::shared_ptr<Junction> startJunction,
    const std::shared_ptr<Junction> endJunction, const uint32_t length,
    interface::LinePainter* const painter)
{
    pathId_++;
    auto startPoint = startJunction->getPosition();
    auto endPoint = endJunction->getPosition();
    calculatePathPoints(startPoint, endPoint, PAVEMENTOFFSET, length);

    auto pavement = std::make_shared<Path>(pathId_, length, startPoint, endPoint, endJunction);
    pavement->setPainter(painter);
    pavement->update();
    startJunction->addOutgoingPavement(pavement);
    pavementConnections_[startJunction->getId()].push_back(pavement);
}

void Simulation::addDriver(interface::PointPainter* const painter)
{
    addDriver(generateRandomDriver(generateRandomVehicle()), std::move(painter));
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    interface::PointPainter* const painter)
{
    addDriver(generateRandomDriver(std::make_unique<Vehicle>(maxAcceleration, maxDeceleration)),
        std::move(painter));
}

void Simulation::addDriver(const float accelerationRate,
    const uint32_t minDistanceToVehicleAhead, const uint32_t maxSpeedOverLimit,
    const float roadConditionSpeedModifier, interface::PointPainter* const painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_, generateRandomVehicle(),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit, roadConditionSpeedModifier),
        std::move(painter));
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
    const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
    interface::PointPainter* const painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_,
        std::make_unique<Vehicle>(maxAcceleration, maxDeceleration),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit, roadConditionSpeedModifier),
        std::move(painter));
}

void Simulation::addPedestrian(interface::PointPainter* const painter)
{
    auto maxSpeed = static_cast<uint32_t>(std::rand() % 901 + 300); /* 300 - 12000 */
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), std::move(painter));
}

void Simulation::addPedestrian(const uint32_t maxSpeed, interface::PointPainter* const painter)
{
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), std::move(painter));
}

void Simulation::updateObjects()
{
    for(const auto& junction : junctions_)
    {
        junction->update();
    }
    for(const auto& roads : roadConnections_)
    {
        for(const auto& road : roads.second)
        {
            road->update();
        }
    }
    for(const auto& pavements : pavementConnections_)
    {
        for(const auto& pavement : pavements.second)
        {
            pavement->update();
        }
    }
    for(const auto& driver : drivers_)
    {
        driver->update();
    }
    for(const auto& pedestrian : pedestrians_)
    {
        pedestrian->update();
    }
}

void Simulation::calculatePathPoints(common::Point &startPoint, common::Point &endPoint,
    const uint32_t offset, const uint32_t length)
{
    const auto startJunctionPosition = startPoint;
    const auto endJunctionPosition = endPoint;

    if(startPoint.y > endPoint.y)
    {
        startPoint.x += offset;
        endPoint.x += offset;
    }
    else
    {
        startPoint.x -= offset;
        endPoint.x -= offset;
    }

    if(startPoint.x > endPoint.x)
    {
        startPoint.y -= offset;
        endPoint.y -= offset;
    }
    else
    {
        startPoint.y += offset;
        endPoint.y += offset;
    }

    const auto junctionRadiusSquared = (JUNCTIONRADIUS - 2) * (JUNCTIONRADIUS - 2);
    auto startPointShift = common::Point{static_cast<int16_t>(endPoint.x - startPoint.x),
        static_cast<int16_t>(endPoint.y - startPoint.y)};
    auto currentLength = length;
    auto traversedLength = 0;

    endPoint = startPoint;
    while(currentLength/2 > 0)
    {
        currentLength /= 2;

        auto changeOnX = endPoint.x - endJunctionPosition.x;
        auto changeOnY = endPoint.y - endJunctionPosition.y;
        if((changeOnX * changeOnX) + (changeOnY * changeOnY) > junctionRadiusSquared)
        {
            traversedLength += currentLength;
        }
        else
        {
            traversedLength -= currentLength;
        }
        auto partTravelled = static_cast<float>(traversedLength) / static_cast<float>(length);
        endPoint.x = startPoint.x + (startPointShift.x * partTravelled);
        endPoint.y = startPoint.y + (startPointShift.y * partTravelled);
    }

    auto endPointShift = common::Point{static_cast<int16_t>(startPoint.x - endPoint.x),
        static_cast<int16_t>(startPoint.y - endPoint.y)};
    currentLength = length;
    traversedLength = 0;

    startPoint = endPoint;
    while(currentLength/2 > 0)
    {
        currentLength /= 2;

        auto changeOnX = startPoint.x - startJunctionPosition.x;
        auto changeOnY = startPoint.y - startJunctionPosition.y;
        if((changeOnX * changeOnX) + (changeOnY * changeOnY) > junctionRadiusSquared)
        {
            traversedLength += currentLength;
        }
        else
        {
            traversedLength -= currentLength;
        }
        auto partTravelled = static_cast<float>(traversedLength) / static_cast<float>(length);
        startPoint.x = endPoint.x + (endPointShift.x * partTravelled);
        startPoint.y = endPoint.y + (endPointShift.y * partTravelled);
    }
}

void Simulation::generateBaseSimulation()
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, BASEENDPOINT);
    junctions_.push_back(junction);
    roadConnections_[junctionId_] = {};
    pavementConnections_[junctionId_] = {};

    auto startPointRoad = BASESTARTPOINT;
    auto endPointRoad = BASEENDPOINT;
    calculatePathPoints(startPointRoad, endPointRoad, ROADOFFSET, SPAWNPATHSLENGTH);
    auto startPointPavement = BASESTARTPOINT;
    auto endPointPavement = BASEENDPOINT;
    calculatePathPoints(startPointPavement, endPointPavement, PAVEMENTOFFSET, SPAWNPATHSLENGTH);

    pathId_++;
    spawnRoad_ = std::make_shared<Road>(pathId_, SPAWNPATHSLENGTH, startPointRoad,
        endPointRoad, junction, RoadCondition::NoPotHoles, 70);

    pathId_++;
    spawnPavement_ = std::make_shared<Path>(pathId_, SPAWNPATHSLENGTH, startPointPavement,
        endPointPavement, junction);
}

std::unique_ptr<Vehicle> Simulation::generateRandomVehicle() const
{
    auto maxAcceleration = static_cast<uint32_t>(std::rand() % 4 + 5); /* 5 - 8 */
    auto maxDeceleration = static_cast<uint32_t>(std::rand() % 15 + 10); /* 14 - 24 */

    return std::make_unique<Vehicle>(maxAcceleration, maxDeceleration);
}

std::shared_ptr<Driver> Simulation::generateRandomDriver(std::unique_ptr<Vehicle> vehicle) const
{
    const auto accelerationRate = static_cast<float>(std::rand()) / ( static_cast<float>(RAND_MAX/(0.8)))
        + float{0.2}; /* 0.2 - 1 */
    const auto minDistanceToVehicleAhead = static_cast<uint32_t>(std::rand() % 8001 + uint32_t{1000}); /* 1000 - 8000 */
    const auto maxSpeedOverLimit = static_cast<uint32_t>(std::rand() % 31); /* 0 - 30 */
    const auto roadConditionSpeedModifier = static_cast<float>(std::rand())
        / ( static_cast<float>(RAND_MAX/(0.15))); /* 0 - 0.15 */

    return std::make_shared<Driver>(spawnRoad_, std::move(vehicle), accelerationRate,
        minDistanceToVehicleAhead, maxSpeedOverLimit, roadConditionSpeedModifier);
}

void Simulation::addDriver(std::shared_ptr<Driver> driver,
    interface::PointPainter* const painter)
{
    driver->setPainter(std::move(painter));
    drivers_.push_back(driver);
}

void Simulation::addPedestrian(std::shared_ptr<Pedestrian> pedestrian,
    interface::PointPainter* const painter)
{
    pedestrian->setPainter(std::move(painter));
    pedestrians_.push_back(pedestrian);
}

} // trafficsimulation::model
