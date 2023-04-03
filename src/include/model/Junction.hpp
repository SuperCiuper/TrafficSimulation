/*!
 *
 */

#ifndef JUNCTION_HPP
#define JUNCTION_HPP

#include <map>
#include <memory>
#include <vector>

#include <QObject>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class PointPainter; }

namespace trafficsimulation::model
{

class Path;
class Road;

/*!
 * \brief The Junction class
 */
class Junction : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructor
     * \param junctionId - id of conctructed Junction
     * \param position - position of constructed Junction
     *
     * Starts timer for light change
     */
    Junction(const uint32_t junctionId, const common::Point position);
    /*!
     * Constructor for temporary Junction (for path that cross other Junction)
     * \param junctionId - id of conctructed Junction
     * \param position - position of constructed Junction
     * \param pathWithGreenLightId - path which has green light
     */
    Junction(const uint32_t junctionId, const common::Point position, const uint32_t pathWithGreenLightId);
    ~Junction();

    uint32_t getId() const;
    common::Point getPosition() const;
    uint32_t getSpeedLimit() const;
    bool isGreenLight(const uint32_t pathId) const;
    const std::vector<std::shared_ptr<Road>>& getOutgoingRoads() const;
    const std::vector<std::shared_ptr<Path>>& getOutgoingPavements() const;
    const std::shared_ptr<Road> getFastestRoad(const uint32_t destinationId);
    const std::shared_ptr<Path> getFastestPavement(const uint32_t destinationId);

    void addIncomingRoadId(const uint32_t roadId);
    void addOutgoingRoad(const std::shared_ptr<Road> newRoad);
    void addOutgoingPavement(const std::shared_ptr<Path> newPavement);
    void setFastestRoute(const uint32_t destinationId,
        const std::pair<std::shared_ptr<Road>, std::shared_ptr<Path>> bestPaths);

    std::shared_ptr<Road> createTemporaryRoad(const common::Point startPoint,
        const std::shared_ptr<Road> newRoad) const;
    std::shared_ptr<Path> createTemporaryPavement(const common::Point startPoint,
        const std::shared_ptr<Path> newPavement) const;

    void setPainter(std::unique_ptr<interface::PointPainter> painter);
    void update();

private:
    void changeLights();

    const uint32_t junctionId_;
    const common::Point position_;
    const uint32_t speedLimit_;

    std::vector<uint32_t> incomingRoadPathIds_;
    uint32_t roadWithGreenIterator_;
    bool roadGreenLight_;
    bool pavementGreenLight_;
    bool wasLastGreenLightRoad_;
    std::vector<std::shared_ptr<Road>> outgoingRoads_;
    std::vector<std::shared_ptr<Path>> outgoingPavements_;
    std::map<uint32_t /* destinationId */, std::pair<std::shared_ptr<Road>, std::shared_ptr<Path>>> fastestRoutes_;

    std::unique_ptr<interface::PointPainter> painter_;
};

} // trafficsimulation::model

#endif // JUNCTION_HPP