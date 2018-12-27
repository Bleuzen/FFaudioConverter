/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018  Bleuzen
https://github.com/Bleuzen/FFaudioConverter
supgesu@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------- */

#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include "translator.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Bleuzen");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion("0.17");

    qDebug() << "Starting" << QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion();

    a.setWindowIcon(QIcon(":/com.github.Bleuzen.FFaudioConverter.ico"));

#ifdef Q_OS_WIN
    a.setStyle(QStyleFactory::create("Fusion"));  // Use Fusion style on Windows because it looks better
#endif

    // Register ENUMs
    qRegisterMetaType<FFmpegTask::ConvertStatus>("FFmpegTask::ConvertStatus");

    // Load settings
    Settings::init();

    // Load translations
    Translator::init();

    // Show GUI
    MainWindow w;
    w.show();

    return a.exec();
}
