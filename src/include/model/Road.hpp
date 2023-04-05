#ifndef ROAD_HPP
#define ROAD_HPP

#include "Path.hpp"
#include "RoadCondition.hpp"

namespace trafficsimulation::model
{

class Vehicle;

class Road : public Path
{
public:
    Road(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
        const common::Point endPoint, const std::shared_ptr<Junction> endJunction,
        const RoadCondition roadCondition, const uint32_t speedLimit);
    ~Road();

    RoadCondition getRoadCondition() const;
    uint32_t getSpeedLimit() const;
    const std::shared_ptr<Vehicle> getLastVehicle() const;
    void addVehicle(std::shared_ptr<Vehicle> newLastVehicle);

private:
    const RoadCondition roadCondition_;
    const uint32_t speedLimit_;
    std::weak_ptr<Vehicle> lastVehicle_;
};

} // trafficsimulation::model

#endif // ROAD_HPP
