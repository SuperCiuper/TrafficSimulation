#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <map>
#include <memory>
#include <vector>

#include <QObject>
#include <QTimer>

namespace trafficsimulation::common{ class Point; }

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
class Road;
enum class RoadCondition;
class Vehicle;

class Simulation : public QObject
{
    Q_OBJECT

public:
    Simulation();
    ~Simulation();

    void setBasePrinters(interface::PointPainter* const junctionPainter,
        interface::LinePainter* const roadPainter,
        interface::LinePainter* const pavementPainter);
    std::optional<std::string> start();
    void stop();
    bool isRunning();

    const std::vector<std::shared_ptr<Junction>>& getJunctions() const;
    const std::vector<std::shared_ptr<Driver>>& getDrivers() const;
    const std::vector<std::shared_ptr<Pedestrian>>& getPedestrians() const;
    const std::map<uint32_t, std::vector<uint32_t>> getConnectedJunctionsByRoad() const;
    const std::map<uint32_t, std::vector<uint32_t>> getConnectedJunctionsByPavement() const;

    void addJunction(const common::Point position, interface::PointPainter* const painter);
    void addRoad(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        const RoadCondition roadCondition, const uint32_t speedLimit,
        interface::LinePainter* const painter);
    void addPavement(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        interface::LinePainter* const painter);

    void addDriver(interface::PointPainter* const painter);
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        interface::PointPainter* const painter);
    void addDriver(const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
        interface::PointPainter* const painter);
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
        interface::PointPainter* const painter);

    void addPedestrian(interface::PointPainter* const painter);
    void addPedestrian(const uint32_t maxSpeed, interface::PointPainter* const painter);

private:
    void updateObjects();

    void calculateFastestRoutes();
    void calculatePathPoints(common::Point& startPoint, common::Point& endPoint,
        const uint32_t offset, const uint32_t length);

    void generateBaseSimulation();
    std::unique_ptr<Vehicle> generateRandomVehicle() const;
    std::shared_ptr<Driver> generateRandomDriver(std::unique_ptr<Vehicle> vehicle) const;
    void addDriver(std::shared_ptr<Driver> driver, interface::PointPainter* const painter);
    void addPedestrian(std::shared_ptr<Pedestrian> pedestrian,
        interface::PointPainter* const painter);

    uint32_t junctionId_;
    uint32_t pathId_;
    std::vector<std::shared_ptr<Junction>> junctions_;
    std::vector<std::shared_ptr<Driver>> drivers_;
    std::vector<std::shared_ptr<Pedestrian>> pedestrians_;
    std::map<uint32_t, std::vector<std::shared_ptr<Road>>> roadConnections_;
    std::map<uint32_t, std::vector<std::shared_ptr<Path>>> pavementConnections_;
    std::shared_ptr<Road> spawnRoad_;
    std::shared_ptr<Path> spawnPavement_;

    std::unique_ptr<QTimer> simulationRefreshTimer_;
    bool basePrintersSet_;
};

} // trafficsimulation::model

#endif // SIMULATION_HPP
