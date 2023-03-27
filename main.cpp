#include <QApplication>
#include <QTimer>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    auto timer = QTimer{};

    return a.exec();
}
