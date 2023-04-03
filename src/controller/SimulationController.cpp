#include "../include/controller/SimulationController.hpp"

#include <QMessageBox>

#include "../include/MainWindow.hpp"
#include "../include/common/Point.hpp"
#include "../include/interface/PointPainter.hpp"
#include "../include/interface/LinePainter.hpp"
#include "../include/model/Junction.hpp"
#include "../include/model/Simulation.hpp"

#include "../include/view/dialogs/CreateJunctionDialog.hpp"

namespace trafficsimulation::controller
{

constexpr auto EDGEOFFSET = uint32_t{20};

SimulationController::SimulationController(MainWindow *mainWindow)
    : mainWindow_{mainWindow}
    , simulation_{nullptr}
{
    createSimulation();
}

SimulationController::~SimulationController() = default;

void SimulationController::addJunction()
{
    auto dialog = view::dialogs::CreateJunctionDialog{EDGEOFFSET, SCENEWIDTH + EDGEOFFSET,
        SCENEHEIGHT - EDGEOFFSET};
    if(dialog.exec() == QDialog::Accepted)
    {
        simulation_->addJunction(dialog.getPoint(), mainWindow_->addJunctionPainter());
    }
}

void SimulationController::addPavement()
{
    auto junctions = simulation_->getJunctions();
    auto filteredJunctions = std::vector<std::pair<uint32_t, common::Point>>{};
    std::transform(junctions.cbegin(), junctions.cend(), std::back_inserter(filteredJunctions),
        [](const std::shared_ptr<model::Junction>& junction){
            return std::make_pair<uint32_t, common::Point>(junction->getId(), junction->getPosition());
        });


}

void SimulationController::addRoad()
{
    //simulation_->addJunction()
}

void SimulationController::addDriver()
{

}

void SimulationController::addPedestrian()
{

}

void SimulationController::createSimulation()
{
    simulation_ = std::make_unique<model::Simulation>();
    simulation_->setBasePrinters(std::move(mainWindow_->addJunctionPainter()),
        std::move(mainWindow_->addRoadPainter()), std::move(mainWindow_->addPavementPainter()));
}

void SimulationController::resetSimulation()
{

}

} // trafficsimulation::controller
