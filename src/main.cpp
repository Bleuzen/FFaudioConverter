/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018-2024  Bleuzen
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
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Bleuzen");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion(VERSIONSTR);
    QGuiApplication::setDesktopFileName("com.github.Bleuzen.FFaudioConverter");

    QCommandLineParser commandLine;
    commandLine.addHelpOption();
    commandLine.addVersionOption();
    commandLine.addPositionalArgument("path", "Open with files or directories added");
    QCommandLineOption noTranslationsOption("no-translations", "Do not load translations");
    commandLine.addOption(noTranslationsOption);
    commandLine.process(app);

    qDebug().noquote() << "Starting" << QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion();

    app.setWindowIcon(QIcon(":/com.github.Bleuzen.FFaudioConverter.ico"));

    // Register ENUMs
    qRegisterMetaType<FFmpegTask::ConvertStatus>("FFmpegTask::ConvertStatus");

    // Load settings
    Settings::init();

    // Load translations
    if (!commandLine.isSet(noTranslationsOption)) Translator::init();

    // Show GUI
    MainWindow w;
    w.show();

    if(commandLine.positionalArguments().length() > 0) {
        w.processCommandLine(commandLine.positionalArguments());
    }

    return app.exec();
}
