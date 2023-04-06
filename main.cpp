#include <QApplication>

#include "src/include/mainwindow.hpp"

int main(int argc, char *argv[])
{
    auto application = new QApplication{argc, argv};
    auto mainWindow = new trafficsimulation::MainWindow{};
    mainWindow->show();

    auto result = application->exec();

    delete mainWindow;
    delete application;

    return result;
}
