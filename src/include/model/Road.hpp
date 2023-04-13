#ifndef ROAD_HPP
#define ROAD_HPP

#include "Path.hpp"
#include "RoadCondition.hpp"

namespace trafficsimulation::model
{

class Vehicle;

/*!
 * \brief The Road class
 *
 * Inherits and extends Path class.
 * Is more advanced path between Junction objects. Has speedLimit and condition properties
 * Used by Driver and (by extention) Vehicle objects
 */
class Road : public Path
{
public:
    /*!
     * \brief Constructor
     * \param uint32_t pathId
     * \param uint32_t length
     * \param common::Point startPoint
     * \param common::Point endPoint
     * \param std::shared_ptr<Junction> endJunction
     * \param RoadCondition roadCondition
     * \param uint32_t speedLimit
     */
    Road(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
        const common::Point endPoint, const std::shared_ptr<Junction> endJunction,
        const RoadCondition roadCondition, const uint32_t speedLimit);
    ~Road();

    RoadCondition getRoadCondition() const;
    uint32_t getSpeedLimit() const;

    /*!
     * \brief addVehicle
     * \param Vehicle* newLastVehicle
     *
     * Called when Vehicle object enters this Road.
     * Informs newLastVehicle if there is a Vehicle in front of it on this Road
     */
    void addVehicle(Vehicle* const newLastVehicle);
    /*!
     * \brief removeVehicle
     * \param Vehicle* removedVehicle
     *
     * Called when Vehicle object leaves this Road.
     */
    void removeVehicle(Vehicle* const removedVehicle);

private:
    const RoadCondition roadCondition_;
    const uint32_t speedLimit_;
    Vehicle* lastVehicle_;
};

} // trafficsimulation::model

#endif // ROAD_HPP
