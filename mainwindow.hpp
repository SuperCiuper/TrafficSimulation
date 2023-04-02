#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "path.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void drawSimulation();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *timer;

    std::vector<QPoint> junctions;
    std::vector<std::vector<int>> connections;
    std::vector<Path *> paths;
};
#endif // MAINWINDOW_HPP
