#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    static QString OutputFormat;
    static bool ChangeSamplerate;
    static bool Force16bitFLAC;
    static bool Multithreading;
};

#endif // SETTINGS_H
