/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018 Elias Martin (Bleuzen)
https://github.com/Bleuzen/FFaudioConverter
supgesu@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------- */

#include "settings.h"

QString Settings::FFmpegBinary;
QString Settings::OutputFormat;
QString Settings::Quality;
QString Settings::OutputDirectory;
bool Settings::ChangeSamplerate;
QString Settings::OutputSamplerate;
bool Settings::SkipExistingFiles;
bool Settings::Multithreading;

void Settings::init() {
    QSettings settings;
    Settings::FFmpegBinary = settings.value("FFmpegBinary", DEFAULT_FFMPEG_BINARY).toString();
    Settings::OutputFormat = settings.value("OutputFormat", "mp3").toString();
    Settings::Quality = settings.value("Quality", "High").toString();
    Settings::OutputDirectory = settings.value("OutputDirectory", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + QCoreApplication::applicationName()).toString();
    Settings::ChangeSamplerate = settings.value("ChangeSamplerate", false).toBool();
    Settings::OutputSamplerate = settings.value("OutputSamplerate", "44100").toString();
    Settings::SkipExistingFiles = settings.value("SkipExistingFiles", true).toBool();
    Settings::Multithreading = settings.value("Multithreading", true).toBool();
    qDebug() << "Settings loaded";
}
