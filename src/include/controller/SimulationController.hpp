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

class SimulationController
{
public:
    SimulationController(MainWindow* mainWindow);
    ~SimulationController();

    void addJunction();
    void addPavement();
    void addRoad();
    void addDriver();
    void addPedestrian();

    void createSimulation();
    void resetSimulation();

private:

    std::vector<view::dialogs::NotConnectedJunction> findNotConnectedJunctions(
        std::map<uint32_t, std::vector<uint32_t>> connectedJunctions);

    MainWindow* mainWindow_;
    std::unique_ptr<model::Simulation> simulation_;
};

} // trafficsimulation::controller

#endif // SIMULATIONCONTROLLER_HPP
