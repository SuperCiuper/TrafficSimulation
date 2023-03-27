#ifndef JUNCTION_HPP
#define JUNCTION_HPP

#include <map>
#include <memory>
#include <vector>

#include "Path.hpp"
#include "Point.hpp"
#include "Road.hpp"

/*!
 * \brief The Junction class
 */
class Junction
{
public:
    /*!
     * \brief Junction
     * \param junctionId
     * \param position
     */
    Junction(const uint32_t junctionId, const Point position);
    /*!
     * \brief Junction
     * \param junctionId
     * \param position
     * \param pathWithGreenLightId
     */
    Junction(const uint32_t junctionId, const Point position, const uint32_t pathWithGreenLightId);
    ~Junction();

    uint32_t getId() const;
    Point getPosition() const;
    bool isGreenLight(const uint32_t pathId) const;
    const std::vector<std::shared_ptr<Road>>& getOutgoingRoads() const;
    const std::vector<std::shared_ptr<Path>>& getOutgoingPavements() const;

    template<class T>
    const std::shared_ptr<T>getFastestRoute(const uint32_t destinationId);

    std::shared_ptr<Road> createTemporaryRoad(const Point startPoint,
        const std::shared_ptr<Road> oldRoad) const;
    std::shared_ptr<Path> createTemporaryPavement(const Point startPoint,
        const std::shared_ptr<Path> oldPavement) const;

    void addIncomingRoadId(const uint32_t);
    void addOutgoingRoad(const std::shared_ptr<Road> newRoad);
    void addOutgoingPavement(const std::shared_ptr<Path> newPavement);
    void setFastestRoute(const uint32_t destinationId,
        const std::pair<std::shared_ptr<Road>, std::shared_ptr<Path>> bestPath);

private:
    void changeLights();

    const uint32_t junctionId_;
    const Point position_;
    const std::vector<uint32_t>incomingRoadPathIds_;
    uint32_t roadWithGreenLight_;
    bool pavementGreenLight_;
    std::vector<std::shared_ptr<Road>> outgoingRoads_;
    std::vector<std::shared_ptr<Path>> outgoingPavements_;
    std::map<uint32_t /* destinationId */, std::pair<std::shared_ptr<Road>, std::shared_ptr<Path>>> fastestRoutes_;
};

#endif // JUNCTION_HPP
