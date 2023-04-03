#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsScene;

namespace trafficsimulation::controller { class SimulationController; }
namespace trafficsimulation::interface
{
class PointPainter;
class LinePainter;
}

namespace trafficsimulation
{

constexpr auto SCENEWIDTH = uint32_t{1300};
constexpr auto SCENEHEIGHT = uint32_t{820};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // void resetScene(); not needed for now
    std::unique_ptr<interface::PointPainter> addJunctionPainter();
    std::unique_ptr<interface::PointPainter> addDriverPainter();
    std::unique_ptr<interface::PointPainter> addPedestrianPainter();
    std::unique_ptr<interface::LinePainter> addRoadPainter();
    std::unique_ptr<interface::LinePainter> addPavementPainter();

private:
    Ui::MainWindow *ui_;
    QGraphicsScene *scene_;
    std::unique_ptr<controller::SimulationController> controller_;
};

} // trafficsimulation::view

#endif // MAINWINDOW_HPP
