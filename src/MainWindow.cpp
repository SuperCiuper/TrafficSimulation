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
    scene_->installEventFilter(this);

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
}

MainWindow::~MainWindow()
{
    delete scene_;
    delete ui_;
}

std::unique_ptr<interface::PointPainter> MainWindow::addJunctionPainter()
{
    auto painter = new view::JunctionPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<interface::PointPainter>(painter);
}

std::unique_ptr<interface::PointPainter> MainWindow::addDriverPainter()
{
    auto painter = new view::DriverPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<interface::PointPainter>(painter);
}

std::unique_ptr<interface::PointPainter> MainWindow::addPedestrianPainter()
{
    auto painter = new view::PedestrianPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<interface::PointPainter>(painter);
}

std::unique_ptr<interface::LinePainter> MainWindow::addRoadPainter()
{
    auto painter = new view::RoadPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<interface::LinePainter>(painter);
}

std::unique_ptr<interface::LinePainter> MainWindow::addPavementPainter()
{
    auto painter = new view::PavementPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<interface::LinePainter>(painter);
}

} // trafficsimulation
