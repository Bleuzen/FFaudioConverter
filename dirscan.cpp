#include "dirscan.h"


DirScan::DirScan()
{
}

QList<QString> DirScan::ListFiles(QString path)
{
    files.clear();

    scanDir(path);

    return files;
}

void DirScan::scanDir(QDir dir)
{
    //dir.setNameFilters(QStringList("*.nut"));

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++)
    {
        QString filePath = dir.path() + QDir::separator() + fileList[i];
        files.append(filePath);
    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i)
    {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        scanDir(QDir(newPath));
    }
}
