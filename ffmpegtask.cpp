#include "ffmpegtask.h"
#include "mainwindow.h"

FFmpegTask::FFmpegTask(int id, QString inpath, QString outdir)
{
    this->id = id;
    this->infile = inpath;
    this->outdir = outdir;
}

void FFmpegTask::run()
{
    //qDebug() << "New FFmpeg task started";

    qDebug() << "Convert:" << id << infile;

    QFileInfo fileInfo(infile);
    QDir qdir = fileInfo.absoluteDir();

    QString dir = qdir.path();
    //QString basename = fileInfo.baseName();
    QString name = fileInfo.fileName();
    QString basename = name.left(name.lastIndexOf("."));
    //path = fileInfo.filePath();  // path is already set

    // add dir path of the input file to the outdir to reconstruct directory structure
    outdir = (outdir + dir);

    QString outfile = outdir + QDir::separator() + basename + ".mp3";

    // Create output dir of not exists
    //TODO: need to do this every time?
    QDir().mkpath(outdir);

    qDebug() << "To:" << outfile;

    // Convert using ffmpeg command
    QProcess *ffmpeg = new QProcess();
    ffmpeg->setProgram("ffmpeg");
    ffmpeg->setArguments(QStringList() << "-hide_banner" << "-n"
                         << "-i"
                         << infile
                         << "-c:a" << "libmp3lame"
                         << "-q:a" << "1"
                         << "-ar" << "44100"
                         << "-map_metadata" << "0"
                         << "-map_metadata" << "0:s:0"
                         << "-id3v2_version" << "3"
//                         << "-vn"
                         << outfile);
    ffmpeg->start();
    ffmpeg->waitForFinished();
    int exitCode = ffmpeg->exitCode();

    qDebug() << "Exit code:" << exitCode;

    bool success = (exitCode == 0);

    //qDebug() << "FFmpeg task finished";

    emit Done(id, success);
}
