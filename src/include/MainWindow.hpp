#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsScene;

namespace trafficsimulation::controller { class SimulationController; }
namespace trafficsimulation::view
{
class PointPainter;
class PathPainter;
}

namespace trafficsimulation
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // void resetScene(); not needed for now
    std::unique_ptr<view::PointPainter> addJunctionPainter();
    std::unique_ptr<view::PointPainter> addDriverPainter();
    std::unique_ptr<view::PointPainter> addPedestrianPainter();
    std::unique_ptr<view::PathPainter> addRoadPainter();
    std::unique_ptr<view::PathPainter> addPavementPainter();

private:
    Ui::MainWindow *ui_;
    QGraphicsScene *scene_;
    std::unique_ptr<controller::SimulationController> controller_;
};

} // trafficsimulation::view

#endif // MAINWINDOW_HPP
