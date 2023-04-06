#include "include/MainWindow.hpp"
#include "ui_mainwindow.h"

#include <QGraphicsScene>

#include "include/view/PathPainters.hpp"
#include "include/view/PointPainters.hpp"
#include "include/controller/SimulationController.hpp"

#include <iostream>

namespace trafficsimulation
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , scene_{new QGraphicsScene(this)}
    , controller_{std::make_unique<controller::SimulationController>(this)}
{
    ui_->setupUi(this);
    ui_->graphicsView->setScene(scene_);
    ui_->graphicsView->installEventFilter(new GraphicsViewFilter{});

    scene_->setSceneRect(0, 0, SCENEWIDTH, SCENEHEIGHT);

    auto line1 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().topRight());
    auto line2 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().bottomLeft());
    scene_->addLine(line1);
    scene_->addLine(line2);

    connect(ui_->addJunctionButton, &QPushButton::clicked,
        this, [this](){ controller_->addJunction(); });
    connect(ui_->addPavementButton, &QPushButton::clicked,
        this, [this](){ controller_->addPavement(); });
    connect(ui_->addRoadButton, &QPushButton::clicked,
        this, [this](){ controller_->addRoad(); });
    connect(ui_->addDriverButton, &QPushButton::clicked,
        this, [this](){ controller_->addDriver(); });
    connect(ui_->addPedestrianButton, &QPushButton::clicked,
        this, [this](){ controller_->addPedestrian(); });
    connect(ui_->startStopButton, &QPushButton::clicked,
        this, &MainWindow::handleStartStopButtonClick);
    connect(ui_->resetButton, &QPushButton::clicked,
        this, [this](){ controller_->resetSimulation(); });
}

MainWindow::~MainWindow()
{
    delete scene_;
    delete ui_;
}

void MainWindow::resetScene()
{
    scene_->clear();

    auto line1 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().topRight());
    auto line2 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().bottomLeft());
    scene_->addLine(line1);
    scene_->addLine(line2);
}

interface::PointPainter* MainWindow::addJunctionPainter()
{
    auto painter = new view::JunctionPainter{};
    scene_->addItem(painter);
    return painter;
}

interface::PointPainter* MainWindow::addDriverPainter()
{
    auto painter = new view::DriverPainter{};
    scene_->addItem(painter);
    return painter;
}

interface::PointPainter* MainWindow::addPedestrianPainter()
{
    auto painter = new view::PedestrianPainter{};
    scene_->addItem(painter);
    return painter;
}

interface::LinePainter* MainWindow::addRoadPainter()
{
    auto painter = new view::RoadPainter{};
    scene_->addItem(painter);
    return painter;
}

interface::LinePainter* MainWindow::addPavementPainter()
{
    auto painter = new view::PavementPainter{};
    scene_->addItem(painter);
    return painter;
}

void MainWindow::handleStartStopButtonClick()
{
    std::cout<< " handleClick " << std::endl;
    if(controller_->isSimulationRunning())
    {
        std::cout<< " XD " << std::endl;
        controller_->stopSimulation();
        ui_->startStopButton->setText("Start");

        for(const auto& button : {ui_->addJunctionButton, ui_->addPavementButton,
            ui_->addRoadButton, ui_->addDriverButton, ui_->addPedestrianButton, ui_->resetButton})
        {
            button->setEnabled(true);
        }
        return;
    }
    if(controller_->startSimulation())
    {
        ui_->startStopButton->setText("Stop");

        for(const auto& button : {ui_->addJunctionButton, ui_->addPavementButton,
            ui_->addRoadButton, ui_->addDriverButton, ui_->addPedestrianButton, ui_->resetButton})
        {
            button->setEnabled(false);
        }
    }
}

} // trafficsimulation
