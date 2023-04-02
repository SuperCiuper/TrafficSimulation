#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "qgraphicsitem.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , junctions{}
    , connections{{1, 2, 4}, {0, 3, 4}, {0, 3, 4}, {1, 2, 4}, {0, 1, 2, 3}}
    , paths{}
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene->setSceneRect(0, 0, 1300, 820);
    auto line1 = QLineF(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    auto line2 = QLineF(scene->sceneRect().topLeft(), scene->sceneRect().bottomLeft());

    scene->addLine(line1);
    scene->addLine(line2);

    const auto junctionRadius = qreal{36};
    const auto roadOffset = int{4};
    auto pen = QPen(Qt::blue, 1, Qt::SolidLine);
    junctions.push_back(QPoint(20, 20));
    junctions.push_back(QPoint(20, 800));
    junctions.push_back(QPoint(1280, 20));
    junctions.push_back(QPoint(1280, 800));
    junctions.push_back(QPoint(650, 410));

    for(auto i = 0; i < std::size(connections); i++)
    {
        for(const auto& endPoint : connections[i])
        {
            qInfo() << junctions[i].x() << "    " << junctions[endPoint].x();
            paths.push_back(new Path(junctions[i], junctions[endPoint], Qt::blue, roadOffset));
            scene->addItem(paths[std::size(paths) - 1]);
            ui->pathSelectorComboBox->addItem(QString::fromStdString(std::to_string(std::size(paths) - 1)));
            paths.push_back(new Path(junctions[i], junctions[endPoint], Qt::gray, roadOffset * 2 + 1));
            scene->addItem(paths[std::size(paths) - 1]);
            ui->pathSelectorComboBox->addItem(QString::fromStdString(std::to_string(std::size(paths) - 1)));
        }
    }
    pen.setColor(Qt::black);
    for(const auto& point : junctions)
    {
        scene->addEllipse(point.x() - junctionRadius/2, point.y() - junctionRadius/2,
            junctionRadius, junctionRadius, pen, QBrush(Qt::green));
    }

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::drawSimulation);
    timer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawSimulation()
{
    std::cout << "drawing XD" << std::endl;
    for(const auto& item : paths)
    {
        item->update();
    }
};


