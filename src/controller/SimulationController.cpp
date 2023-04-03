#include "../include/controller/SimulationController.hpp"

#include <QMessageBox>

#include "../include/MainWindow.hpp"
#include "../include/model/Simulation.hpp"

#include <iostream>

namespace trafficsimulation::controller
{

SimulationController::SimulationController(MainWindow *mainWindow)
    : mainWindow_{mainWindow}
    , simulation_{nullptr}
{
    createSimulation();
}

SimulationController::~SimulationController() = default;

void SimulationController::addJunction()
{
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.exec();

    std::cout << "xd" << std::endl;
}

void SimulationController::addRoad()
{
    //simulation_->addJunction()
}

void SimulationController::createSimulation()
{
    simulation_ = std::make_unique<model::Simulation>();
    // simulation_->setBasePrinters(mainWindow_->addJunctionPointer());
}

void SimulationController::resetSimulation()
{

}

} // trafficsimulation::controller
