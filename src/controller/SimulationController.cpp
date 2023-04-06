#include "../include/controller/SimulationController.hpp"

#include <QMessageBox>
#include <QString>

#include "../include/MainWindow.hpp"
#include "../include/common/Point.hpp"
#include "../include/interface/PointPainter.hpp"
#include "../include/interface/LinePainter.hpp"
#include "../include/model/Junction.hpp"
#include "../include/model/Simulation.hpp"

#include "../include/view/dialogs/CreateDriverDialog.hpp"
#include "../include/view/dialogs/CreateJunctionDialog.hpp"
#include "../include/view/dialogs/CreatePavementDialog.hpp"
#include "../include/view/dialogs/CreatePedestrianDialog.hpp"
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
    const auto& connectedJunctions = simulation_->getConnectedJunctionsByPavement();
    const auto& notConnectedJunctions = findNotConnectedJunctions(connectedJunctions);

    if(std::size(notConnectedJunctions) == 0)
    {
        return;
    }

    auto dialog = view::dialogs::CreatePavementDialog{notConnectedJunctions, mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        auto junctions = simulation_->getJunctions();
        auto startJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [startId = dialog.getStartId()](const auto j){ return j->getId() == startId; });
        auto endJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [endId = dialog.getEndId()](const auto j){ return j->getId() == endId; });

        simulation_->addPavement(*startJunction, *endJunction, dialog.getLength(),
            mainWindow_->addPavementPainter());

        if(dialog.bothDirections())
        {
            simulation_->addPavement(*endJunction, *startJunction, dialog.getLength(),
                mainWindow_->addPavementPainter());
        }
    }
}

void SimulationController::addRoad()
{
    const auto& connectedJunctions = simulation_->getConnectedJunctionsByRoad();
    const auto& notConnectedJunctions = findNotConnectedJunctions(connectedJunctions);

    if(std::size(notConnectedJunctions) == 0)
    {
        return;
    }

    auto dialog = view::dialogs::CreateRoadDialog{notConnectedJunctions, mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        auto junctions = simulation_->getJunctions();
        auto startJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [startId = dialog.getStartId()](const auto j){ return j->getId() == startId; });
        auto endJunction = std::find_if(junctions.cbegin(), junctions.cend(),
            [endId = dialog.getEndId()](const auto j){ return j->getId() == endId; });

        simulation_->addRoad(*startJunction, *endJunction, dialog.getLength(),
            dialog.getRoadCondition(), dialog.getSpeedLimit(),
            mainWindow_->addRoadPainter());

        if(dialog.bothDirections())
        {
            simulation_->addRoad(*endJunction, *startJunction, dialog.getLength(),
                dialog.getRoadCondition(), dialog.getSpeedLimit(),
                mainWindow_->addRoadPainter());
        }
    }
}

void SimulationController::addDriver()
{
    auto dialog = view::dialogs::CreateDriverDialog{mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog.isVehicleRandomized() && dialog.isDriverRandomized())
        {
            simulation_->addDriver(mainWindow_->addPedestrianPainter());
            return;
        }
        if(dialog.isVehicleRandomized())
        {
            simulation_->addDriver(dialog.getAccelerationRate(), dialog.getMinDistanceToVehicle(),
                dialog.getMaxSpeedOverLimit(), dialog.getRoadConditionSpeedModifier(),
                mainWindow_->addPedestrianPainter());
            return;
        }
        if(dialog.isDriverRandomized())
        {
            simulation_->addDriver(dialog.getMaxAcceleration(), dialog.getMaxDeceleration(),
                mainWindow_->addPedestrianPainter());
            return;
        }
        simulation_->addDriver(dialog.getMaxAcceleration(), dialog.getMaxDeceleration(),
            dialog.getAccelerationRate(), dialog.getMinDistanceToVehicle(), dialog.getMaxSpeedOverLimit(),
            dialog.getRoadConditionSpeedModifier(), mainWindow_->addPedestrianPainter());
    }
}

void SimulationController::addPedestrian()
{
    auto dialog = view::dialogs::CreatePedestrianDialog{mainWindow_};
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog.isRandomized())
        {
            simulation_->addPedestrian(mainWindow_->addPedestrianPainter());
            return;
        }
        simulation_->addPedestrian(dialog.getMaxSpeed(), mainWindow_->addPedestrianPainter());
    }
}

bool SimulationController::startSimulation()
{
    auto result = simulation_->start();
    if(result.has_value())
    {
        auto msgBox = new QMessageBox{mainWindow_};
        msgBox->setText(QString::fromStdString(result.value()));
        msgBox->exec();
        return false;
    }
    return true;
}

void SimulationController::stopSimulation()
{
    simulation_->stop();
}

bool SimulationController::isSimulationRunning()
{
    return simulation_->isRunning();
}

void SimulationController::resetSimulation()
{
    simulation_ = nullptr;
    mainWindow_->resetScene();
    createSimulation();
}

void SimulationController::createSimulation()
{
    simulation_ = std::make_unique<model::Simulation>();
    simulation_->setBasePrinters(std::move(mainWindow_->addJunctionPainter()),
        std::move(mainWindow_->addRoadPainter()), std::move(mainWindow_->addPavementPainter()));
}

std::vector<view::dialogs::NotConnectedJunction> SimulationController::findNotConnectedJunctions(
    std::map<uint32_t, std::vector<uint32_t>> connectedJunctions)
{
    auto notConnectedJunctions = std::vector<view::dialogs::NotConnectedJunction>{};
    auto junctions = simulation_->getJunctions();
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
    }
    return notConnectedJunctions;
}

} // trafficsimulation::controller
