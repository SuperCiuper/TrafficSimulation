#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <map>
#include <memory>
#include <vector>

#include "Driver.hpp"
#include "Junction.hpp"
#include "Path.hpp"
#include "Pedestrian.hpp"
#include "Point.hpp"
#include "Road.hpp"

class Simulation
{
public:
    Simulation();
    ~Simulation();

    const std::vector<std::shared_ptr<Junction>>& getJunctions() const;
    const std::vector<std::shared_ptr<Driver>>& getDrivers() const;
    const std::vector<std::shared_ptr<Pedestrian>>& getPedestrians() const;

    void addJunction(const Point);
    void addRoad(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        const RoadCondition roadCondition, const uint32_t speedLimit);
    void addPavement(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length);
    void addDriver();
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration);
    void addDriver(const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit);
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit);
    void addPedestrian();
    void addPedestrian(const uint32_t maxSpeed);

private:
    void generateBaseSimulation();
    void calculateFastestRoutes();
    void updateObjects();

    std::vector<std::shared_ptr<Junction>> junctions_;
    std::vector<std::shared_ptr<Driver>> drivers_;
    std::vector<std::shared_ptr<Pedestrian>> pedestrians_;
    std::map<uint32_t, std::vector<std::shared_ptr<Road>>> roadConnections_;
    std::map<uint32_t, std::vector<std::shared_ptr<Path>>> pavementConnections_;
    const std::shared_ptr<Road> spawnRoad_;
    const std::shared_ptr<Path> spawnPavement_;
};

#endif // SIMULATION_HPP
