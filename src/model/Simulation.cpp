#include "../../include/model/Simulation.hpp"

#include <cstdlib>
#include <ctime>

#include "../../include/interface/LinePainter.hpp"
#include "../../include/interface/PointPainter.hpp"

#include "../../include/model/Driver.hpp"
#include "../../include/model/Junction.hpp"
#include "../../include/model/Path.hpp"
#include "../../include/model/Pedestrian.hpp"
#include "../../include/model/Road.hpp"
#include "../../include/model/Vehicle.hpp"

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

void Simulation::setBasePrinters(const std::shared_ptr<interface::PointPainter> junctionPainter,
    const std::shared_ptr<interface::LinePainter> roadPainter,
    const std::shared_ptr<interface::LinePainter> pavementPainter)
{
    junctions_.front()->setPainter(junctionPainter);
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

void Simulation::addJunction(const Point position, const std::shared_ptr<interface::PointPainter> painter)
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, position);
    junction->setPainter(painter);
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
    calculateOffset(startPoint, endPoint, ROADOFFSET);

    auto road = std::make_shared<Road>(pathId_, length, startPoint, endPoint,
        endJunction, roadCondition, speedLimit);
    road->setPainter(painter);
    roadConnections_[startJunction->getId()].push_back(road);
}

void Simulation::addPavement(const std::shared_ptr<Junction> startJunction,
    const std::shared_ptr<Junction> endJunction, const uint32_t length,
    const std::shared_ptr<interface::LinePainter> painter)
{
    pathId_++;
    auto startPoint = startJunction->getPosition();
    auto endPoint = endJunction->getPosition();
    calculateOffset(startPoint, endPoint, PAVEMENTOFFSET);

    auto pavement = std::make_shared<Path>(pathId_, length, startPoint, endPoint, endJunction);
    pavement->setPainter(painter);
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

void Simulation::calculateOffset(Point &startPoint, Point &endPoint, uint32_t offset)
{
    if(startPoint.y > endPoint.y)
    {
        startPoint.x -= offset;
        endPoint.x -= offset;
    }
    else
    {
        startPoint.x += offset;
        endPoint.x += offset;
    }

    if(startPoint.x < endPoint.x)
    {
        startPoint.y -= offset;
        endPoint.y -= offset;
        return;
    }
    startPoint.y += offset;
    endPoint.y += offset;
}

void Simulation::generateBaseSimulation()
{
    junctionId_++;
    auto junction = std::make_shared<Junction>(junctionId_, Point{20, 20});
    junctions_.push_back(junction);

    auto startPointRoad = Point{-200, -200};
    auto endPointRoad = Point{20, 20};
    calculateOffset(startPointRoad, startPointRoad, ROADOFFSET);
    auto startPointPavement = Point{-200, -200};
    auto endPointPavement = Point{20, 20};
    calculateOffset(startPointPavement, endPointPavement, PAVEMENTOFFSET);

    pathId_++;
    spawnRoad_ =  std::make_shared<Road>(pathId_, SPAWNPATHSLENGTH, startPointRoad,
        endPointRoad, junction, RoadCondition::NoPotHoles, 70);

    pathId_++;
    spawnPavement_ =  std::make_shared<Path>(pathId_, SPAWNPATHSLENGTH, startPointPavement,
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
