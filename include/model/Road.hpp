#ifndef ROAD_HPP
#define ROAD_HPP

#include "Path.hpp"
#include "Vehicle.hpp"

enum class RoadCondition
{
    New,
    NoPotHoles,
    SomePotHoles,
    LotsOfPotHoles,
    Offroad
};

class Road : public Path
{
public:
    Road(const uint32_t pathId, const uint32_t length_, const Point startPoint,
        const Point endPoint, const std::shared_ptr<Junction> endJunction,
        const RoadCondition roadCondition, const uint32_t speedLimit);
    ~Road();

    RoadCondition getRoadCondition() const;
    uint32_t getSpeedLimit() const;
    void addVehicle(std::shared_ptr<Vehicle> newLastVehicle);

private:
    const RoadCondition roadCondition_;
    const uint32_t speedLimit_;
    const std::shared_ptr<Vehicle> lastVehicle_;
};

#endif // ROAD_HPP
