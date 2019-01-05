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

#include "settings.h"

QTranslator Translator::qtTranslator;
QTranslator Translator::appTranslator;

void Translator::init() {
    QString defaultLocale = QLocale::system().name();
    loadTranslations(defaultLocale);
}

void Translator::loadTranslations(QString language) {
    qDebug() << "Loading translations for" << language;
    // Load Qt translation
    if(qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qApp->installTranslator(&qtTranslator);
        qDebug() << "Qt translation loaded";
    } else {
        qDebug() << "Qt translation not found";
    }
    // Load app translation
    if(appTranslator.load(language, ":/i18n/")) {
        qApp->installTranslator(&appTranslator);
        qDebug() << "App translation loaded";
    } else {
        qDebug() << "App translation not found";
    }
}
