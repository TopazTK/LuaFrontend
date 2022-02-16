#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication App(argc, argv);
    App.setStyle("Fusion");

    MainWindow w;
    w.show();

    return App.exec();
}
