#include "../include/controller/SimulationController.hpp"

#include <QMessageBox>

#include "../include/MainWindow.hpp"
#include "../include/common/Point.hpp"
#include "../include/interface/PointPainter.hpp"
#include "../include/interface/LinePainter.hpp"
#include "../include/model/Junction.hpp"
#include "../include/model/Simulation.hpp"

#include "../include/view/dialogs/CreateJunctionDialog.hpp"
#include "../include/view/dialogs/CreatePavementDialog.hpp"
#include "../include/view/dialogs/CreateRoadDialog.hpp"

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
        SCENEHEIGHT - EDGEOFFSET, mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        simulation_->addJunction(dialog.getPoint(), mainWindow_->addJunctionPainter());
    }
}

void SimulationController::addPavement()
{
    auto junctions = simulation_->getJunctions();
    auto connectedJunctions = simulation_->getConnectedJunctionsByRoad();
    auto notConnectedJunctions = std::vector<view::dialogs::NotConnectedJunction>{};
    for(const auto& startJunction : junctions)
    {
        auto notConnectedJunction = view::dialogs::NotConnectedJunction{};
        notConnectedJunction.startJunction.junctionId = startJunction->getId();
        notConnectedJunction.startJunction.position = startJunction->getPosition();

        auto currentConnectedJunctions = connectedJunctions[startJunction->getId()];
        for(const auto& endJunction : junctions)
        {
            if(std::find(currentConnectedJunctions.cbegin(), currentConnectedJunctions.cend(),
                endJunction->getId()) == std::cend(currentConnectedJunctions)
                && endJunction->getId() != startJunction->getId())
            {
                auto trimmedJunction = view::dialogs::Junction{};
                trimmedJunction.junctionId = endJunction->getId();
                trimmedJunction.position = endJunction->getPosition();
                notConnectedJunction.endJunctions.push_back(trimmedJunction);
            }
        }
        if(std::size(notConnectedJunction.endJunctions) != 0)
        notConnectedJunctions.push_back(notConnectedJunction);
    }
    if(std::size(notConnectedJunctions) == 0)
    {
        auto msgBox = new QMessageBox{mainWindow_};
        msgBox->setText("All junctions are connected! Add new junction");
        msgBox->exec();
        return;
    }

    auto dialog = view::dialogs::CreatePavementDialog{notConnectedJunctions, mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        auto startJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [startId = dialog.getStartId()](const auto j){ return j->getId() == startId; });
        auto endJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [endId = dialog.getEndId()](const auto j){ return j->getId() == endId; });

        simulation_->addPavement(*startJunction, *endJunction, dialog.getLength(),
            mainWindow_->addPavementPainter());
    }
}

void SimulationController::addRoad()
{

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
