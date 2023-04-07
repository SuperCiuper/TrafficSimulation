#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QEvent>

#include "view/dialogs/NotConnectedJunction.hpp"

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

/* reduce unnecessary repaints */
class GraphicsViewFilter : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if(event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate)
        {
            return true;
        }
        return QObject::eventFilter(obj, event);
    };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resetScene();
    void setDestinations(const std::vector<view::dialogs::Junction> destinationJunctions);
    uint32_t getDestination() const;

    interface::PointPainter* addJunctionPainter();
    interface::PointPainter* addDriverPainter();
    interface::PointPainter* addPedestrianPainter();
    interface::LinePainter* addRoadPainter();
    interface::LinePainter* addPavementPainter();

private:
    void handleStartStopButtonClick();

    Ui::MainWindow *ui_;
    QGraphicsScene *scene_;
    std::unique_ptr<controller::SimulationController> controller_;
    std::vector<view::dialogs::Junction> destinationJunctions_;
};

} // trafficsimulation::view

#endif // MAINWINDOW_HPP
