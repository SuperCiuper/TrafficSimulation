#include <QApplication>

#include "src/include/mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    trafficsimulation::MainWindow w;
    w.show();

    return a.exec();
}
