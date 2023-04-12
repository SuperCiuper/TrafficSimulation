#ifndef SIMULATIONCONTROLLER_HPP
#define SIMULATIONCONTROLLER_HPP

#include <map>
#include <memory>
#include <vector>

namespace trafficsimulation{ class MainWindow; }
namespace trafficsimulation::model{ class Simulation; }
namespace trafficsimulation::view::dialogs{ struct NotConnectedJunction; }

namespace trafficsimulation::controller
{

/*!
 * \brief The SimulationController class
 *
 * Controls program, connects view with model and handles user input logic
 */
class SimulationController
{
public:
    /*!
     * \brief Constructor
     * \param MainWindow* mainWindow
     *
     * Sets pointer to mainWindow, creates new simulation
     */
    SimulationController(MainWindow* mainWindow);
    ~SimulationController();

    /*!
     * \brief addJunction
     *
     * Opens CreateJunctionDialog then if user provides valid input adds new junction to
     * simulation with its printer
     */
    void addJunction();
    /*!
     * \brief addPavement
     *
     * Opens CreatePavementDialog then if user provides valid input adds new pavement to
     * simulation with its printer
     */
    void addPavement();
    /*!
     * \brief addRoad
     *
     * Opens CreateRoadDialog then if user provides valid input adds new road to
     * simulation with its printer
     */
    void addRoad();
    /*!
     * \brief addDriver
     *
     * Opens CreateDriverDialog then if user provides valid input or chooses randomization
     * adds new driver to simulation with its printer
     */
    void addDriver();
    /*!
     * \brief addJunction
     *
     * Opens CreatePedestrianDialog then if user provides valid input or chooses randomization
     * adds new pedestrian to simulation with its printer
     */
    void addPedestrian();

    /*!
     * \brief generateBasicMap
     *
     * Generates default map with predefined juncions, roads and pavements
     */
    void generateBasicMap();
    /*!
     * \brief setDestination
     *
     * Sets junction that all movingObjects will go to
     */
    void setDestination();

    /*!
     * \brief startSimulation
     * \return true if simulation started succesfuly
     *
     * Starts simulations, shows MessageBox if start failed. Sets possible destinations
     */
    bool startSimulation();
    /*!
     * \brief stopSimulation
     *
     * Stops simulation
     */
    void stopSimulation();
    /*!
     * \brief isSimulationRunning
     * \return true if simulation is running
     */
    bool isSimulationRunning();
    /*!
     * \brief resetSimulation
     *
     * Resets view, deletes current simulation and creates new one in its place
     */
    void resetSimulation();

private:
    /*!
     * \brief createSimulation
     *
     * Creates new simulation
     */
    void createSimulation();
    /*!
     * \brief findNotConnectedJunctions
     * \param std::map<uint32_t, std::vector<uint32_t>> connectedJunctions
     * \return vector of NotConnectedJunctions
     *
     * Returns vector junctions that are not yet connected with each other
     */
    std::vector<view::dialogs::NotConnectedJunction> findNotConnectedJunctions(
        std::map<uint32_t, std::vector<uint32_t>> connectedJunctions);

    MainWindow* mainWindow_;
    std::unique_ptr<model::Simulation> simulation_;
};

} // trafficsimulation::controller

#endif // SIMULATIONCONTROLLER_HPP
