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
    QFileInfo fileInfo(infile);
    QDir qdir = fileInfo.absoluteDir();

    QString dir = qdir.path();
    QString name = fileInfo.fileName();
    QString basename = name.left(name.lastIndexOf("."));

    // add dir path of the input file to the outdir to reconstruct directory structure
    outdir = (outdir + dir);

    outfile = outdir + QDir::separator() + basename;

    // Create FFmpeg process
    QProcess *ffmpeg = new QProcess();
    ffmpeg->setProgram(Settings::FFmpegBinary);
    ffmpeg->setArguments(buildArgs());

    // Skip file if it has already been converted
    if(Settings::SkipExistingFiles) {
        QFileInfo outfileinfo(outfile);
        if(outfileinfo.exists()) {
            // File already exists, no need to convert it again
            emit ConvertDone(id, ConvertStatus::Skipped);
            return;
        }
    }

    qDebug() << "Convert:" << id << infile;

    // Create output dir if it does not exist
    //TODO: need to do this every time?
    QDir().mkpath(outdir);

    qDebug() << "To:" << outfile;

    // Start FFmpeg process
    ffmpeg->start();
    ffmpeg->waitForFinished();
    int exitCode = ffmpeg->exitCode();

    qDebug() << id << "FFmpeg exit code:" << exitCode;

    bool success = (exitCode == 0);

    if(success) {
        emit ConvertDone(id, ConvertStatus::Done);
    } else {
        emit ConvertDone(id, ConvertStatus::Failed);
    }
}

QStringList FFmpegTask::buildArgs() {
    QStringList ffmpeg_args;
    ffmpeg_args << "-hide_banner";
    if(Settings::SkipExistingFiles) {
        ffmpeg_args<< "-n";  // Not overwrite (keep existing file)
    } else {
        ffmpeg_args<< "-y";  // Overwrite (reconvert if file exists)
    }
    ffmpeg_args << "-i" << infile;

    if(Settings::OutputFormat == "mp3") {
        outfile += ".mp3";
        // mp3 options
        ffmpeg_args << "-c:a" << "libmp3lame";
        ffmpeg_args << "-q:a" << "1";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << Settings::OutputSamplerate;
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";
        ffmpeg_args << "-id3v2_version" << "3";

    } else if(Settings::OutputFormat == "ogg") {
        outfile += ".ogg";
        // ogg options
        ffmpeg_args << "-vn"; //TODO: remove this to keep album art but without the output is a video, will have to find a solution
        ffmpeg_args << "-c:a" << "libvorbis";
        ffmpeg_args << "-q:a" << "9";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << Settings::OutputSamplerate;
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "flac") {
        outfile += ".flac";
        // flac options
        ffmpeg_args << "-c:a" << "flac";
        if (Settings::Force16bitFLAC) ffmpeg_args << "-sample_fmt" << "s16";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << Settings::OutputSamplerate;
        ffmpeg_args << "-map_metadata" << "0";
        ffmpeg_args << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "wav") {
        outfile += ".wav";
        // wav options
        ffmpeg_args << "-c:a" << "pcm_s16le";
        if (Settings::ChangeSamplerate) ffmpeg_args << "-ar" << Settings::OutputSamplerate;

    } else {
        // unknown format options
        outfile += "." + Settings::OutputFormat;
    }

    ffmpeg_args << outfile;

    return ffmpeg_args;
}
