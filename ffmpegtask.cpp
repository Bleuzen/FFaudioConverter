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
    qDebug() << "Convert:" << id << infile;

    QFileInfo fileInfo(infile);
    QDir qdir = fileInfo.absoluteDir();

    QString dir = qdir.path();
    QString name = fileInfo.fileName();
    QString basename = name.left(name.lastIndexOf("."));

    // add dir path of the input file to the outdir to reconstruct directory structure
    outdir = (outdir + dir);

    outfile = outdir + QDir::separator() + basename + ".";

    // Create output dir of not exists
    //TODO: need to do this every time?
    QDir().mkpath(outdir);

    qDebug() << "To:" << outfile;

    // Convert using ffmpeg command
    QProcess *ffmpeg = new QProcess();
    ffmpeg->setProgram("ffmpeg");
    ffmpeg->setArguments(buildArgs());
    ffmpeg->start();
    ffmpeg->waitForFinished();
    int exitCode = ffmpeg->exitCode();

    qDebug() << id << "FFmpeg exit code:" << exitCode;

    bool success = (exitCode == 0);

    emit Done(id, success);
}

QStringList FFmpegTask::buildArgs() {
    QStringList ffmpeg_args;
    ffmpeg_args << "-hide_banner" << "-n"
                             << "-i"
                             << infile
                             << "-c:a" << "libmp3lame"
                             << "-q:a" << "1"
                             << "-ar" << "44100"
                             << "-map_metadata" << "0"
                             << "-map_metadata" << "0:s:0"
                             << "-id3v2_version" << "3"
                             << outfile;
    return ffmpeg_args;
}
