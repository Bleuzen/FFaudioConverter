#include "mainwindow.h"
#include <QApplication>

#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Bleuzen");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion("0.9.0");

    qDebug() << "Starting" << QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion();

    // Register ENUMs
    qRegisterMetaType<FFmpegTask::ConvertStatus>("FFmpegTask::ConvertStatus");

    // Load settings
    Settings::init();

    // Show GUI
    MainWindow w;
    w.show();

    return a.exec();
}
