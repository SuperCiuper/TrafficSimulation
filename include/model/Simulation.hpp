#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <map>
#include <memory>
#include <vector>

namespace trafficsimulation::interface
{
class LinePainter;
class PointPainter;
}

namespace trafficsimulation::model
{

class Driver;
class Junction;
class Path;
class Pedestrian;
class Point;
class Road;
enum class RoadCondition;
class Vehicle;

class Simulation
{
public:
    Simulation();
    ~Simulation();

    void generateBaseSimulation();
    void startSimulation();

    const std::vector<std::shared_ptr<Junction>>& getJunctions() const;
    const std::vector<std::shared_ptr<Driver>>& getDrivers() const;
    const std::vector<std::shared_ptr<Pedestrian>>& getPedestrians() const;

    void addJunction(const Point position, const std::shared_ptr<interface::PointPainter> painter);
    void addRoad(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        const RoadCondition roadCondition, const uint32_t speedLimit,
        const std::shared_ptr<interface::LinePainter> painter);
    void addPavement(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        const std::shared_ptr<interface::LinePainter> painter);

    void addDriver(const std::shared_ptr<interface::PointPainter> painter);
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        const std::shared_ptr<interface::PointPainter> painter);
    void addDriver(const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const std::shared_ptr<interface::PointPainter> painter);
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const std::shared_ptr<interface::PointPainter> painter);

    void addPedestrian(const std::shared_ptr<interface::PointPainter> painter);
    void addPedestrian(const uint32_t maxSpeed, const std::shared_ptr<interface::PointPainter> painter);

private:
    void updateObjects();

    void calculateFastestRoutes();
    void calculateOffset(Point& startPoint, Point& endPoint, uint32_t offset);

    std::shared_ptr<Vehicle> generateRandomVehicle() const;
    std::shared_ptr<Driver> generateRandomDriver(const std::shared_ptr<Vehicle> vehicle) const;
    void addDriver(std::shared_ptr<Driver> driver, const std::shared_ptr<interface::PointPainter> painter);
    void addPedestrian(std::shared_ptr<Pedestrian> pedestrian,
        const std::shared_ptr<interface::PointPainter> painter);

    uint32_t junctionId_;
    uint32_t pathId_;
    std::vector<std::shared_ptr<Junction>> junctions_;
    std::vector<std::shared_ptr<Driver>> drivers_;
    std::vector<std::shared_ptr<Pedestrian>> pedestrians_;
    std::map<uint32_t, std::vector<std::shared_ptr<Road>>> roadConnections_;
    std::map<uint32_t, std::vector<std::shared_ptr<Path>>> pavementConnections_;
    std::shared_ptr<Road> spawnRoad_;
    std::shared_ptr<Path> spawnPavement_;
};

} // trafficsimulation::model

#endif // SIMULATION_HPP
