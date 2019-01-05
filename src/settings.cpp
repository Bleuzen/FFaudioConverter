/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018  Elias Martin (Bleuzen)
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

#include "settings.h"

QString Settings::FFmpegBinary;
QString Settings::OutputFormat;
QString Settings::Quality;
QString Settings::OutputDirectory;
QString Settings::OutputSamplerate;
QString Settings::Filters;
bool Settings::UseSoXresampler;
bool Settings::QuickConvertMode;
int Settings::Threads;

void Settings::init() {
    QSettings settings;
    Settings::FFmpegBinary = settings.value("FFmpegBinary", DEFAULT_FFMPEG_BINARY).toString();
    Settings::OutputFormat = settings.value("OutputFormat", "mp3").toString();
    Settings::Quality = settings.value("Quality", "high").toString();
    Settings::OutputDirectory = settings.value("OutputDirectory", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + QCoreApplication::applicationName()).toString();
    Settings::OutputSamplerate = settings.value("OutputSamplerate", "44100").toString();
    Settings::Filters = settings.value("Filters", "").toString();
    Settings::UseSoXresampler = settings.value("UseSoXresampler", false).toBool();
    Settings::QuickConvertMode = settings.value("QuickConvertMode", false).toBool();
    Settings::Threads = settings.value("Threads", 0).toInt();
    qDebug() << "Settings loaded";
}
