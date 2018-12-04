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
    Settings::FFmpegBinary = settings.value("FFmpegBinary", "/usr/bin/ffmpeg").toString();
    Settings::OutputFormat = settings.value("OutputFormat", "mp3").toString();
    Settings::Quality = settings.value("Quality", "High").toString();
    Settings::OutputDirectory = settings.value("OutputDirectory", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + QCoreApplication::applicationName()).toString();
    Settings::ChangeSamplerate = settings.value("ChangeSamplerate", false).toBool();
    Settings::OutputSamplerate = settings.value("OutputSamplerate", "44100").toString();
    Settings::SkipExistingFiles = settings.value("SkipExistingFiles", true).toBool();
    Settings::Multithreading = settings.value("Multithreading", true).toBool();
    qDebug() << "Settings loaded";
}
