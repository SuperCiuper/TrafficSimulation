#include "../../include/model/Simulation.hpp"

#include <cstdlib>
#include <ctime>

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
{
    std::srand(std::time(0));
    generateBaseSimulation();
}

Simulation::~Simulation() = default;

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

void Simulation::addDriver(const std::shared_ptr<interface::PointPainter> painter)
{
    addDriver(generateRandomDriver(generateRandomVehicle()), painter);
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    const std::shared_ptr<interface::PointPainter> painter)
{
    addDriver(generateRandomDriver(std::make_shared<Vehicle>(maxAcceleration, maxDeceleration)),
        painter);
}

void Simulation::addDriver(const float accelerationRate,
    const uint32_t minDistanceToVehicleAhead, const uint32_t maxSpeedOverLimit,
    const std::shared_ptr<interface::PointPainter> painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_, generateRandomVehicle(),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit), painter);
}

void Simulation::addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
    const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
    const uint32_t maxSpeedOverLimit, const std::shared_ptr<interface::PointPainter> painter)
{
    addDriver(std::make_shared<Driver>(spawnRoad_,
        std::make_shared<Vehicle>(maxAcceleration, maxDeceleration),
        accelerationRate, minDistanceToVehicleAhead, maxSpeedOverLimit), painter);
}

void Simulation::addPedestrian(const std::shared_ptr<interface::PointPainter> painter)
{
    auto maxSpeed = static_cast<uint32_t>(std::rand() % 901 + 300); /* 300 - 12000 */
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), painter);
}

void Simulation::addPedestrian(const uint32_t maxSpeed, const std::shared_ptr<interface::PointPainter> painter)
{
    addPedestrian(std::make_shared<Pedestrian>(spawnPavement_, maxSpeed), painter);
}

void Simulation::generateBaseSimulation()
{

}

void Simulation::updateObjects()
{

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

std::shared_ptr<Vehicle> Simulation::generateRandomVehicle() const
{
    auto maxAcceleration = static_cast<uint32_t>(std::rand() % 4 + 5); /* 5 - 8 */
    auto maxDeceleration = static_cast<uint32_t>(std::rand() % 15 + 10); /* 14 - 24 */

    return std::make_shared<Vehicle>(maxAcceleration, maxDeceleration);
}

std::shared_ptr<Driver> Simulation::generateRandomDriver(const std::shared_ptr<Vehicle> vehicle) const
{
    auto accelerationRate = static_cast<float>(std::rand()) / ( static_cast<float>(RAND_MAX/(0.8)))
        + float{0.2}; /* 0.2 - 1 */
    auto minDistanceToVehicleAhead = static_cast<uint32_t>(std::rand() % 8001 + uint32_t{1000}); /* 1000 - 8000 */
    auto maxSpeedOverLimit = static_cast<uint32_t>(std::rand() % 31); /* 0 - 30 */

    return std::make_shared<Driver>(spawnRoad_, vehicle, accelerationRate,
        minDistanceToVehicleAhead, maxSpeedOverLimit);
}

void Simulation::addDriver(std::shared_ptr<Driver> driver,
    const std::shared_ptr<interface::PointPainter> painter)
{
    driver->setPainter(painter);
    drivers_.push_back(driver);
}

void Simulation::addPedestrian(std::shared_ptr<Pedestrian> pedestrian, const std::shared_ptr<interface::PointPainter> painter)
{
    pedestrian->setPainter(painter);
    pedestrians_.push_back(pedestrian);
}


} // trafficsimulation::model
