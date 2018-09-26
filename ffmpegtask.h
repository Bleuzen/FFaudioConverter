#ifndef FFMPEGTASK_H
#define FFMPEGTASK_H

#include <QDebug>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QFile>
#include <QDir>

class FFmpegTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    FFmpegTask(int id, QString inpath, QString outdir);

private:
    int id;
    QString infile;
    QString outdir;

signals:
    void Done(int id, bool success);

protected:
    void run();
};

#endif // FFMPEGTASK_H
