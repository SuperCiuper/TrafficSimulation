#include "../include/controller/SimulationController.hpp"

#include <map>

#include <QMessageBox>
#include <QString>

#include "../include/MainWindow.hpp"
#include "../include/common/Point.hpp"
#include "../include/interface/PointPainter.hpp"
#include "../include/interface/LinePainter.hpp"
#include "../include/model/Driver.hpp"
#include "../include/model/Junction.hpp"
#include "../include/model/Pedestrian.hpp"
#include "../include/model/RoadCondition.hpp"
#include "../include/model/Simulation.hpp"

#include "../include/view/dialogs/CreateDriverDialog.hpp"
#include "../include/view/dialogs/CreateJunctionDialog.hpp"
#include "../include/view/dialogs/CreatePavementDialog.hpp"
#include "../include/view/dialogs/CreatePedestrianDialog.hpp"
#include "../include/view/dialogs/CreateRoadDialog.hpp"

namespace trafficsimulation::controller
{

namespace
{

struct NewRoad
{
    uint32_t startJunctionIterator;
    uint32_t endJunctionIterator;
    uint32_t lenght;
    model::RoadCondition roadCondition;
    uint32_t speedLimit;
};

}

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
    auto dialog = view::dialogs::CreateJunctionDialog{EDGEOFFSET, SCENEWIDTH - EDGEOFFSET,
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
            const auto connections = simulation_->getConnectedJunctionsByPavement()[(*endJunction)->getId()];

            if(std::find(std::cbegin(connections), std::cend(connections), (*startJunction)->getId())
                == std::cend(connections))
            {
                simulation_->addPavement(*endJunction, *startJunction, dialog.getLength(),
                    mainWindow_->addPavementPainter());
            }
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
            const auto connections = simulation_
                ->getConnectedJunctionsByRoad()[(*endJunction)->getId()];

            if(std::find(std::cbegin(connections), std::cend(connections),
                (*startJunction)->getId()) == std::cend(connections))
            {
                simulation_->addRoad(*endJunction, *startJunction, dialog.getLength(),
                    dialog.getRoadCondition(), dialog.getSpeedLimit(),
                    mainWindow_->addRoadPainter());
            }
        }
        if(dialog.createPavement())
        {
            const auto connections = simulation_
                ->getConnectedJunctionsByPavement()[(*startJunction)->getId()];

            if(std::find(std::cbegin(connections), std::cend(connections),
                (*endJunction)->getId()) == std::cend(connections))
            {
                simulation_->addPavement(*startJunction, *endJunction, dialog.getLength(),
                    mainWindow_->addPavementPainter());
            }

            if(dialog.bothDirections())
            {
                const auto connectionsBackwards = simulation_
                    ->getConnectedJunctionsByPavement()[(*endJunction)->getId()];

                if(std::find(std::cbegin(connectionsBackwards), std::cend(connectionsBackwards),
                    (*startJunction)->getId()) == std::cend(connectionsBackwards))
                {
                    simulation_->addPavement(*endJunction, *startJunction, dialog.getLength(),
                        mainWindow_->addPavementPainter());
                }
            }
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
            simulation_->addDriver(mainWindow_->addDriverPainter());
            return;
        }
        if(dialog.isVehicleRandomized())
        {
            simulation_->addDriver(dialog.getAccelerationRate(), dialog.getMinDistanceToVehicle(),
                dialog.getMaxSpeedOverLimit(), dialog.getRoadConditionSpeedModifier(),
                mainWindow_->addDriverPainter());
            return;
        }
        if(dialog.isDriverRandomized())
        {
            simulation_->addDriver(dialog.getMaxAcceleration(), dialog.getMaxDeceleration(),
                mainWindow_->addDriverPainter());
            return;
        }
        simulation_->addDriver(dialog.getMaxAcceleration(), dialog.getMaxDeceleration(),
            dialog.getAccelerationRate(), dialog.getMinDistanceToVehicle(), dialog.getMaxSpeedOverLimit(),
            dialog.getRoadConditionSpeedModifier(), mainWindow_->addDriverPainter());
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

void SimulationController::generateBasicMap()
{
    const auto BASICMAPPOINTS = std::vector<common::Point>{{SCENEWIDTH - EDGEOFFSET, EDGEOFFSET},
        {EDGEOFFSET, SCENEHEIGHT - EDGEOFFSET}, {SCENEWIDTH - EDGEOFFSET, SCENEHEIGHT - EDGEOFFSET},
        {SCENEWIDTH / 2, SCENEHEIGHT / 2}};

    const auto newRoads = std::vector<NewRoad>{
        {0, 1, 1000, model::RoadCondition::NoPotHoles, 70},
        {0, 2, 350, model::RoadCondition::New, 50},
        {0, 4, 500, model::RoadCondition::Offroad, 50},
        {1, 3, 2000, model::RoadCondition::SomePotHoles, 70},
        {1, 4, 1000, model::RoadCondition::New, 90},
        {2, 3, 500, model::RoadCondition::SomePotHoles, 30},
        {2, 4, 2000, model::RoadCondition::LotsOfPotHoles, 120}};

    auto msgBoxText = QString{"Added following:\n"
        "Junctions; 0: [" + QString::number(EDGEOFFSET) + ", " + QString::number(EDGEOFFSET) + "]"};

    for(auto iterator = 0; iterator < std::size(BASICMAPPOINTS); ++iterator)
    {
        msgBoxText.append("; ").append(QString::number(iterator + 1)).append(": [")
            .append(QString::number(BASICMAPPOINTS[iterator].x)).append(", ")
            .append(QString::number(BASICMAPPOINTS[iterator].y)).append("]");
    }
    msgBoxText.append("\n");
    for(const auto& newRoad : newRoads)
    {
        msgBoxText.append("Road between ").append(QString::number(newRoad.startJunctionIterator))
            .append(" - ").append(QString::number(newRoad.endJunctionIterator))
            .append("; length: ").append(QString::number(newRoad.lenght))
            .append("m, road condition: ").append(QString::fromStdString(toString(newRoad.roadCondition)))
            .append(", speed limit: ").append(QString::number(newRoad.speedLimit)).append("\n");
    }
    msgBoxText.append("All roads are 2 both directions and have pavements");

    auto msgBox = new QMessageBox{mainWindow_};
    msgBox->setText(msgBoxText);
    msgBox->exec();

    for(const auto point : BASICMAPPOINTS)
    {
        simulation_->addJunction(point, mainWindow_->addJunctionPainter());
    }
    auto junctions = simulation_->getJunctions();

    for(const auto& newRoad : newRoads)
    {
        simulation_->addRoad(junctions[newRoad.startJunctionIterator],
            junctions[newRoad.endJunctionIterator], newRoad.lenght * 1000, newRoad.roadCondition,
            newRoad.speedLimit * 10, mainWindow_->addRoadPainter());

        simulation_->addRoad(junctions[newRoad.endJunctionIterator],
            junctions[newRoad.startJunctionIterator], newRoad.lenght * 1000, newRoad.roadCondition,
            newRoad.speedLimit * 10, mainWindow_->addRoadPainter());

        simulation_->addPavement(junctions[newRoad.startJunctionIterator],
            junctions[newRoad.endJunctionIterator], newRoad.lenght * 1000,
            mainWindow_->addPavementPainter());

        simulation_->addPavement(junctions[newRoad.endJunctionIterator],
            junctions[newRoad.startJunctionIterator], newRoad.lenght * 1000,
            mainWindow_->addPavementPainter());
    }
}

void SimulationController::setDestination()
{
    auto destination = mainWindow_->getDestination();
    for(const auto& driver : simulation_->getDrivers())
    {
        driver->setDestination(destination);
    }
    for(const auto& pedestrian : simulation_->getPedestrians())
    {
        pedestrian->setDestination(destination);
    }
}

bool SimulationController::startSimulation()
{
    auto result = simulation_->start(mainWindow_->getRefreshTimeout());
    if(result.has_value())
    {
        auto msgBox = new QMessageBox{mainWindow_};
        msgBox->setText(QString::fromStdString(result.value()));
        msgBox->exec();
        return false;
    }
    auto junctions = std::vector<view::dialogs::Junction>{};
    for(const auto& junction : simulation_->getJunctions())
    {
        junctions.push_back({junction->getId(), junction->getPosition()});
    }
    mainWindow_->setDestinations(junctions);
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
