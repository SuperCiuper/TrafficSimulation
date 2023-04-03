#ifndef SIMULATIONCONTROLLER_HPP
#define SIMULATIONCONTROLLER_HPP

#include <memory>

namespace trafficsimulation{ class MainWindow; }
namespace trafficsimulation::model{ class Simulation; }

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
    MainWindow* mainWindow_;
    std::unique_ptr<model::Simulation> simulation_;
};

} // trafficsimulation::controller

#endif // SIMULATIONCONTROLLER_HPP
