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

#include "translator.h"

#include <QTranslator>
#include <QLocale>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QDebug>

void Translator::init() {
    // Load Qt translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);
    // Load app translations
    QTranslator appTranslator;
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf("_")); // e.g. "de"
    bool translationAvailable = appTranslator.load(QString(":/translations/i18n_%1.qm").arg(defaultLocale));
    if (translationAvailable) qApp->installTranslator(&appTranslator);
    qDebug() << "Translations loaded";
}
