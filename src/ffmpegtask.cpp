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

    outfile = outdir + QDir::separator() + basename;

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
                             << infile;

    if(Settings::OutputFormat == "mp3") {
        // mp3 options
        ffmpeg_args << "-c:a" << "libmp3lame";
        ffmpeg_args << "-q:a" << "1";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << "44100";
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";
        ffmpeg_args << "-id3v2_version" << "3";
        ffmpeg_args << outfile + ".mp3";

    } else if(Settings::OutputFormat == "ogg") {
        // ogg options
        ffmpeg_args << "-vn"; //TODO: remove this to keep album art but without the output is a video, will have to find a solution
        ffmpeg_args << "-c:a" << "libvorbis";
        ffmpeg_args << "-q:a" << "9";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << "44100";
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";
        ffmpeg_args << outfile + ".ogg";

    } else if(Settings::OutputFormat == "flac") {
        // wav options
        ffmpeg_args << "-c:a" << "flac";
        if (Settings::Force16bitFLAC) ffmpeg_args << "-sample_fmt" << "s16";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << "44100";
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";
        ffmpeg_args << outfile + ".flac";

    } else if(Settings::OutputFormat == "wav") {
        // wav options
        ffmpeg_args << "-c:a" << "pcm_s16le";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << "44100";
        ffmpeg_args << outfile + ".wav";

    } else {
        // unknown format options
        ffmpeg_args << outfile + "." + Settings::OutputFormat;
    }

    return ffmpeg_args;
}
