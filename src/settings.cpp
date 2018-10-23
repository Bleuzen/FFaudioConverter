#include "settings.h"

// Settings with their defaults

QString Settings::OutputFormat = "mp3";
QString Settings::OutputDirectory = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + "FFaudioConverter";
bool Settings::ChangeSamplerate = false;
bool Settings::Force16bitFLAC = false;
bool Settings::Multithreading = true;
