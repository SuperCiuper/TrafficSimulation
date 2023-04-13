/*!
 *
 */

#ifndef JUNCTION_HPP
#define JUNCTION_HPP

#include <map>
#include <memory>
#include <vector>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class PointPainter; }

namespace trafficsimulation::model
{

constexpr auto JUNCTIONRADIUS = uint32_t{20};

class Path;
class Road;

/*!
 * \brief The LightState enum
 *
 * Light sequence
 */
enum class LightState
{
    PedestrianGreenLight,
    PedestrianRedLight,
    DriverGreenLight,
    DriverRedLight
};

/*!
 * \brief The Junction class
 *
 * Is at the end of each path, has light logic - allows only objects from one path to
 * cross it at given moment. Has internal paths that are used to cross it.
 */
class Junction
{
public:
    /*!
     * Constructor
     * \param uint32_t junctionId
     * \param common::Point position
     *
     * Randomly generates value of timeoutTicks_.
     */
    Junction(const uint32_t junctionId, const common::Point position);
    ~Junction();

    uint32_t getId() const;
    common::Point getPosition() const;
    uint32_t getSpeedLimit() const;
    /*!
     * \brief isGreenLight
     * \param uint32_t pathId
     * \return true if path with given pathId has green light
     *
     * When pedestrian light is on pedestrians from all paths can cross the junction but
     * drivers can only do so from road that currently has green light
     */
    bool isGreenLight(const uint32_t pathId) const;
    std::vector<std::weak_ptr<Road>> getOutgoingRoads() const;
    std::vector<std::weak_ptr<Path>> getOutgoingPavements() const;
    std::shared_ptr<Road> getFastestRoad(const uint32_t destinationId);
    std::shared_ptr<Path> getFastestPavement(const uint32_t destinationId);

    /*!
     * \brief addIncomingRoad
     * \param std::shared_ptr<Road> newRoad
     *
     * Adds road to incomingRoads_ and creates new junction road for each outgoingRoads_
     * creating connection between them
     */
    void addIncomingRoad(const std::shared_ptr<Road> newRoad);
    /*!
     * \brief addOutgoingRoad
     * \param std::shared_ptr<Road> newRoad
     *
     * Adds road to outgoingRoads_ and creates new junction road for each incomingRoads_
     * creating connection between them
     */
    void addOutgoingRoad(const std::shared_ptr<Road> newRoad);
    /*!
     * \brief addOutgoingPavement
     * \param std::shared_ptr<Path> newPavement
     *
     * Adds pavement to outgoingPavements_
     */
    void addOutgoingPavement(const std::shared_ptr<Path> newPavement);
    /*!
     * \brief setFastestRoute
     * \param uint32_t destinationId
     * \param std::pair<std::weak_ptr<Road>, std::weak_ptr<Path>> bestPaths
     *
     * Sets fastest route to junction of id == destinationId
     */
    void setFastestRoute(const uint32_t destinationId,
        const std::pair<std::weak_ptr<Road>, std::weak_ptr<Path>> bestPaths);

    /*!
     * \brief getJunctionRoad
     * \param uint32_t oldRoadId
     * \param uint32_t newRoadId
     * \return pointer to junction road which is stored at junctionRoads_[oldRoadId][newRoadId]
     */
    std::shared_ptr<Road> getJunctionRoad(const uint32_t oldRoadId, const uint32_t newRoadId) const;
    /*!
     * \brief createTemporaryPavement
     * \param common::Point startPoint
     * \param std::shared_ptr<Path> newPavement
     * \return path between startPoint and startPoint of newPavement
     */
    std::shared_ptr<Path> createTemporaryPavement(const common::Point startPoint,
        const std::shared_ptr<Path> newPavement) const;

    void setPainter(interface::PointPainter* const painter);
    /*!
     * \brief update
     *
     * Decreases ticksToLightChange_, if is 0 then changes lights.
     * If painter is set paints object
     */
    void update();

private:
    /*!
     * \brief changeLights
     *
     * Changes light and sets ticksToLightChange_ to new value, behavior depends on
     * which light is turned on
     */
    void changeLights();

    const uint32_t junctionId_;
    const common::Point position_;
    const uint32_t speedLimit_;

    LightState lightState_;
    uint32_t roadWithGreenIterator_;
    uint32_t timeoutTicks_;
    uint32_t ticksToLightChange_;

    std::vector<std::weak_ptr<Road>> incomingRoads_;
    std::vector<std::weak_ptr<Road>> outgoingRoads_;
    std::vector<std::weak_ptr<Path>> outgoingPavements_;
    std::map<uint32_t, std::map<uint32_t, std::shared_ptr<Road>>> junctionRoads_;
    std::map<uint32_t /* destinationId */, std::pair<std::weak_ptr<Road>, std::weak_ptr<Path>>> fastestRoutes_;

    interface::PointPainter* painter_;
};

} // trafficsimulation::model

#endif // JUNCTION_HPP
