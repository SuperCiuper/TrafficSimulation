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

constexpr auto ROADOFFSET = uint32_t{4};
constexpr auto PAVEMENTOFFSET = uint32_t{9};
constexpr auto SPAWNPATHSLENGTH = uint32_t{100000};
constexpr auto REFRESHRATE = uint32_t{100};

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

void Simulation::setBasePrinters(std::unique_ptr<interface::PointPainter> junctionPainter,
    const std::shared_ptr<interface::LinePainter> roadPainter,
    const std::shared_ptr<interface::LinePainter> pavementPainter)
{
    junctions_.front()->setPainter(std::move(junctionPainter));
    spawnRoad_->setPainter(roadPainter);
    spawnPavement_->setPainter(pavementPainter);
    basePrintersSet_ = true;
}

void Simulation::startSimulation()
{
    if(!basePrintersSet_)
    {
        throw std::runtime_error("Base printers not set - program stopped!");
    }
    if(simulationRefreshTimer_->isActive())
    {
        simulationRefreshTimer_->start(REFRESHRATE);
    }
}

void Simulation::stopSimulation()
{
    if(simulationRefreshTimer_->isActive())
    {
        simulationRefreshTimer_->stop();
    }
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

    for(const auto& roads : pavementConnections_)
    {
        auto startJunctionId = roads.first;

        for(const auto& road : roads.second)
        {
            connectedJunctions[startJunctionId].push_back(road->getJunction()->getId());
        }
    }
    return connectedJunctions;
}

void Simulation::addJunction(const common::Point position,
    std::unique_ptr<interface::PointPainter> painter)
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, position);
    junction->setPainter(std::move(painter));
    junction->update();
    junctions_.push_back(junction);
}

void Simulation::addRoad(const std::shared_ptr<Junction> startJunction,
    const std::shared_ptr<Junction> endJunction, const uint32_t length,
    const RoadCondition roadCondition, const uint32_t speedLimit,
    const std::shared_ptr<interface::LinePainter> painter)
{
    pathId_++;
    auto startPoint = startJunction->getPosition();
    auto endPoint = endJunction->getPosition();
    calculatePathPoints(startPoint, endPoint, ROADOFFSET, length);

    auto road = std::make_shared<Road>(pathId_, length, startPoint, endPoint,
        endJunction, roadCondition, speedLimit);
    road->setPainter(painter);
    startJunction->update();
    endJunction->update();
    roadConnections_[startJunction->getId()].push_back(road);
}

void Simulation::addPavement(const std::shared_ptr<Junction> startJunction,
    const std::shared_ptr<Junction> endJunction, const uint32_t length,
    const std::shared_ptr<interface::LinePainter> painter)
{
    pathId_++;
    auto startPoint = startJunction->getPosition();
    auto endPoint = endJunction->getPosition();
    calculatePathPoints(startPoint, endPoint, PAVEMENTOFFSET, length);

    auto pavement = std::make_shared<Path>(pathId_, length, startPoint, endPoint, endJunction);
    pavement->setPainter(painter);
    startJunction->update();
    endJunction->update();
    pavementConnections_[startJunction->getId()].push_back(pavement);
}

void Simulation::addDriver(std::unique_ptr<interface::PointPainter> painter)
{
    addDriver(generateRandomDriver(generateRandomVehicle()), std::move(painter));
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    std::unique_ptr<interface::PointPainter> painter)
{
    addDriver(generateRandomDriver(std::make_unique<Vehicle>(maxAcceleration, maxDeceleration)),
        std::move(painter));
}

void Simulation::addDriver(const float accelerationRate,
    const uint32_t minDistanceToVehicleAhead, const uint32_t maxSpeedOverLimit,
    const float roadConditionSpeedModifier, std::unique_ptr<interface::PointPainter> painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_, generateRandomVehicle(),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit, roadConditionSpeedModifier),
        std::move(painter));
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
    const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
    std::unique_ptr<interface::PointPainter> painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_,
        std::make_unique<Vehicle>(maxAcceleration, maxDeceleration),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit, roadConditionSpeedModifier),
        std::move(painter));
}

void Simulation::addPedestrian(std::unique_ptr<interface::PointPainter> painter)
{
    auto maxSpeed = static_cast<uint32_t>(std::rand() % 901 + 300); /* 300 - 12000 */
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), std::move(painter));
}

void Simulation::addPedestrian(const uint32_t maxSpeed, std::unique_ptr<interface::PointPainter> painter)
{
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), std::move(painter));
}

void Simulation::repaint()
{
    auto junction = junctions_.front();

    junction->update();
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
    const auto junctionPosition = endPoint;

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

        auto changeOnX = endPoint.x - junctionPosition.x;
        auto changeOnY = endPoint.y - junctionPosition.y;
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
}

void Simulation::generateBaseSimulation()
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, common::Point{20, 20});
    junctions_.push_back(junction);

    auto startPointRoad = common::Point{-100, -100};
    auto endPointRoad = common::Point{20, 20};
    calculatePathPoints(startPointRoad, endPointRoad, ROADOFFSET, SPAWNPATHSLENGTH);
    auto startPointPavement = common::Point{-100, -100};
    auto endPointPavement = common::Point{20, 20};
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
    std::unique_ptr<interface::PointPainter> painter)
{
    driver->setPainter(std::move(painter));
    drivers_.push_back(driver);
}

void Simulation::addPedestrian(std::shared_ptr<Pedestrian> pedestrian,
    std::unique_ptr<interface::PointPainter> painter)
{
    pedestrian->setPainter(std::move(painter));
    pedestrians_.push_back(pedestrian);
}


} // trafficsimulation::model
