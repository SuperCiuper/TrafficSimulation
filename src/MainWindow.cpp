#include "include/MainWindow.hpp"

#include <QGraphicsScene>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "include/view/PathPainters.hpp"
#include "include/view/PointPainters.hpp"
#include "include/controller/SimulationController.hpp"

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
    ui_->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene_->setSceneRect(0, 0, 1300, 820);

    auto line1 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().topRight());
    auto line2 = QLineF(scene_->sceneRect().topLeft(), scene_->sceneRect().bottomLeft());
    scene_->addLine(line1);
    scene_->addLine(line2);

    // simulation_->setBasePrinters()

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

std::unique_ptr<view::PointPainter> MainWindow::addJunctionPainter()
{
    auto painter = new view::JunctionPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<view::PointPainter>(painter);
}

std::unique_ptr<view::PathPainter> MainWindow::addRoadPainter()
{
    auto painter = new view::RoadPainter{};
    scene_->addItem(painter);
    return std::unique_ptr<view::PathPainter>(painter);
}

} // trafficsimulation
