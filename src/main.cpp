#include "mainwindow.h"
#include <QApplication>

#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Bleuzen");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion("0.6");

    // Register ENUMs
    qRegisterMetaType<FFmpegTask::ConvertStatus>("FFmpegTask::ConvertStatus");

    // Load settings
    Settings::init();

    // Show GUI
    MainWindow w;
    w.show();

    return a.exec();
}
