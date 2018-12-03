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

    enum ConvertStatus {  // registered in "main.cpp"
        Pending,
        Failed,
        Skipped,
        Done
    };

private:
    int id;
    QString infile;
    QString outdir;
    QString outfile;

    QStringList ffmpegArgs;

    void prepare();

signals:
    void ConvertDone(int id, FFmpegTask::ConvertStatus status);

protected:
    void run();
};

#endif // FFMPEGTASK_H
