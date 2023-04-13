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

/*!
 * \brief The Simulation class
 *
 * Main class of model classes, add objects to simulation, sets their painters and
 * takts simulation.
 */
class Simulation : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Simulation
     *
     * Initializes Timer that will run simulation
     */
    Simulation();
    ~Simulation();

    /*!
     * \brief setBasePrinters
     * \param interface::PointPainter* junctionPainter
     * \param interface::LinePainter* roadPainter
     * \param interface::LinePainter* pavementPainter
     *
     * Sets painters for spawnRoad_, spawnPavement_ and their endJunction_
     */
    void setBasePrinters(interface::PointPainter* const junctionPainter,
        interface::LinePainter* const roadPainter,
        interface::LinePainter* const pavementPainter);
    /*!
     * \brief start
     * \param uint32_t timeout
     * \return if there was error starting simulation returns error message
     *
     * Starts simulationRefreshTimer_ with given timeout value
     */
    std::optional<std::string> start(uint32_t timeout);
    /*!
     * \brief stop
     *
     * Stops simulationRefreshTimer_
     */
    void stop();
    /*!
     * \brief isRunning
     * \return true if simulationRefreshTimer_ is running
     */
    bool isRunning();

    const std::vector<std::shared_ptr<Junction>>& getJunctions() const;
    const std::vector<std::shared_ptr<Driver>>& getDrivers() const;
    const std::vector<std::shared_ptr<Pedestrian>>& getPedestrians() const;
    /*!
     * \brief getConnectedJunctionsByRoad
     * \return map of vectors where key is id of junction and vector stores ids of
     * junctions that are connected with it by Road
     */
    std::map<uint32_t, std::vector<uint32_t>> getConnectedJunctionsByRoad() const;
    /*!
     * \brief getConnectedJunctionsByPavement
     * \return map of vectors where key is id of junction and vector stores ids of
     * junctions that are connected with it by Pavement
     */
    std::map<uint32_t, std::vector<uint32_t>> getConnectedJunctionsByPavement() const;

    /*!
     * \brief addJunction
     * \param common::Point position
     * \param interface::PointPainter* painter
     *
     * Creates new Junction on position, sets its painter and adds it to the simulation
     */
    void addJunction(const common::Point position, interface::PointPainter* const painter);
    /*!
     * \brief addRoad
     * \param std::shared_ptr<Junction> startJunction
     * \param std::shared_ptr<Junction> endJunction
     * \param uint32_t length
     * \param RoadCondition roadCondition
     * \param uint32_t speedLimit
     * \param interface::LinePainter* painter
     *
     * Creates new Road from startJunction to endJunction, sets its painter and adds
     * it to the simulation
     */
    void addRoad(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        const RoadCondition roadCondition, const uint32_t speedLimit,
        interface::LinePainter* const painter);
    /*!
     * \brief addPavement
     * \param std::shared_ptr<Junction> startJunction
     * \param std::shared_ptr<Junction> endJunction
     * \param uint32_t length
     * \param interface::LinePainter* painter
     *
     * Creates new Pavement from startJunction to endJunction, sets its painter and adds
     * it to the simulation
     */
    void addPavement(const std::shared_ptr<Junction> startJunction,
        const std::shared_ptr<Junction> endJunction, const uint32_t length,
        interface::LinePainter* const painter);

    /*!
     * \brief addDriver
     * \param interface::PointPainter* painter
     *
     * Creates new Vehicle with random parameters and new Driver with random parameters,
     * then sets Driver painter and adds it to the simulation.
     * Driver starts on spawnRoad_
     */
    void addDriver(interface::PointPainter* const painter);
    /*!
     * \brief addDriver
     * \param uint32_t maxAcceleration
     * \param uint32_t maxDeceleration
     * \param interface::PointPainter* painter
     *
     * Creates new Vehicle with given parameters and new Driver with random parameters,
     * then sets Driver painter and adds it to the simulation.
     * Driver starts on spawnRoad_
     */
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        interface::PointPainter* const painter);
    /*!
     * \brief addDriver
     * \param float accelerationRate
     * \param uint32_t minDistanceToVehicleAhead
     * \param uint32_t maxSpeedOverLimit
     * \param float roadConditionSpeedModifier
     * \param interface::PointPainter* painter
     *
     * Creates new Vehicle with random parameters and new Driver with given parameters,
     * then sets Driver painter and adds it to the simulation.
     * Driver starts on spawnRoad_
     */
    void addDriver(const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
        interface::PointPainter* const painter);
    /*!
     * \brief addDriver
     * \param uint32_t maxAcceleration
     * \param uint32_t maxDeceleration
     * \param float accelerationRate
     * \param uint32_t minDistanceToVehicleAhead
     * \param uint32_t maxSpeedOverLimit
     * \param float roadConditionSpeedModifier
     * \param interface::PointPainter* painter
     *
     * Creates new Vehicle with given parameters and new Driver with given parameters,
     * then sets Driver painter and adds it to the simulation.
     * Driver starts on spawnRoad_
     */
    void addDriver(const uint32_t maxAcceleration, const uint32_t maxDeceleration,
        const float accelerationRate, const uint32_t minDistanceToVehicleAhead,
        const uint32_t maxSpeedOverLimit, const float roadConditionSpeedModifier,
        interface::PointPainter* const painter);

    /*!
     * \brief addPedestrian
     * \param interface::PointPainter* painter
     *
     * Creates new Pedestrian with random maxSpeed (between 40 - 120), then sets its
     * painter and adds it to the simulation.
     * Pedestrian starts on spawnPavement_
     */
    void addPedestrian(interface::PointPainter* const painter);
    /*!
     * \brief addPedestrian
     * \param uint32_t maxSpeed
     * \param interface::PointPainter* painter
     *
     * Creates new Pedestrian with given maxSpeed, then sets its painter and adds
     * it to the simulation.
     * Pedestrian starts on spawnPavement_
     */
    void addPedestrian(const uint32_t maxSpeed, interface::PointPainter* const painter);

private:
    /*!
     * \brief updateObjects
     *
     * Called by simulationRefreshTimer_. Calls update() of objects that are part of
     * simulation
     */
    void updateObjects();

    /*!
     * \brief calculateFastestRoutes
     *
     * Calculates fastest routes between junctions and sets them in each junction
     */
    void calculateFastestRoutes();

    /*!
     * \brief calculateConnections
     * \tparam class T
     * \tparam typename Functor
     * \param std::shared_ptr<Junction> junction
     * \param std::map<uint32_t, std::vector<std::shared_ptr<T>>> connections
     * \param Functor costCalculator
     * \return map of pairs where key is id of end junction, first value is id of
     * end junction and second value is pointer to T (path between junctions)
     *
     * Finds shortest path that leads to each other junction in simulation from junction.
     * Uses Functor to calculate cost of new path as roads have different speed limit
     * but pavements do not
     */
    template<class T, typename Functor>
    std::map<uint32_t, std::pair<uint32_t, std::shared_ptr<T>>>
        calculateConnections(std::shared_ptr<Junction> junction,
            std::map<uint32_t, std::vector<std::shared_ptr<T>>> connections,
            Functor costCalculator);
    /*!
     * \brief calculatePathPoints
     * \param common::Point& startPoint
     * \param common::Point& endPoint
     * \param uint32_t offset
     * \param uint32_t length
     *
     * Moves startPoint and endPoint to be on the edge of junction located there.
     * Calculates circle around each point and then puts them on the edge
     */
    void calculatePathPoints(common::Point& startPoint, common::Point& endPoint,
        const uint32_t offset, const uint32_t length);

    /*!
     * \brief generateBaseSimulation
     *
     * Generates spawnRoad_, spawnPavement_ and their endJunction_.
     * spawnRoad_, spawnPavement_ start from outside of the view (-50, -50)
     */
    void generateBaseSimulation();
    /*!
     * \brief generateRandomVehicle
     * \return
     *
     * Generates new Vehicle with values:
     *  - maxAcceleration between (5 - 8)
     *  - maxDeceleration between (14 - 24)
     */
    std::unique_ptr<Vehicle> generateRandomVehicle() const;
    /*!
     * \brief generateRandomDriver
     * \return
     *
     * Generates random vehicle with:
     *  - accelerationRate between (0.2 - 1)
     *  - minDistanceToVehicleAhead between (1000 - 9000)
     *  - maxSpeedOverLimit between (0 - 300)
     *  - roadConditionSpeedModifier between (0 - 0.15)
     */
    std::shared_ptr<Driver> generateRandomDriver(std::unique_ptr<Vehicle> vehicle) const;
    /*!
     * \brief addDriver
     * \param std::shared_ptr<Driver> driver
     * \param interface::PointPainter* painter
     *
     * Sets drivers painter and adds driver to drivers_
     */
    void addDriver(std::shared_ptr<Driver> driver, interface::PointPainter* const painter);
    /*!
     * \brief addPedestrian
     * \param std::shared_ptr<Pedestrian> pedestrian
     * \param interface::PointPainter* painter
     *
     * Sets pedestrians painter and adds pedestrian to pedestrians_
     */
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
