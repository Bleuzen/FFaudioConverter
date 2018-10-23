#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Bleuzen");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion("0.4");

    MainWindow w;
    w.show();

    return a.exec();
}
