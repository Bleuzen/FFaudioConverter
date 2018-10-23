#ifndef DIRSCAN_H
#define DIRSCAN_H

#include <QtDebug>
#include <QDir>

class DirScan
{
public:
    DirScan();
    static QList<QString> ListFiles(QString path);
private:
    static void scanDir(QDir dir);
};

static QList<QString> files;

#endif // DIRSCAN_H
