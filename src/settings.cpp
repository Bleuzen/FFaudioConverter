#include "settings.h"

QString Settings::FFmpegBinary;
QString Settings::OutputFormat;
QString Settings::OutputDirectory;
bool Settings::ChangeSamplerate;
QString Settings::OutputSamplerate;
bool Settings::Force16bitFLAC;
bool Settings::SkipExistingFiles;
bool Settings::Multithreading;

void Settings::init() {
    QSettings settings;
    Settings::FFmpegBinary = settings.value("FFmpegBinary", "/usr/bin/ffmpeg").toString();
    Settings::OutputFormat = settings.value("OutputFormat", "mp3").toString();
    Settings::OutputDirectory = settings.value("OutputDirectory", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + QCoreApplication::applicationName()).toString();
    Settings::ChangeSamplerate = settings.value("ChangeSamplerate", false).toBool();
    Settings::OutputSamplerate = settings.value("OutputSamplerate", "44100").toString();
    Settings::Force16bitFLAC = settings.value("Force16bitFLAC", false).toBool();
    Settings::SkipExistingFiles = settings.value("SkipExistingFiles", true).toBool();
    Settings::Multithreading = settings.value("Multithreading", true).toBool();
    qDebug() << "Settings loaded";
}
