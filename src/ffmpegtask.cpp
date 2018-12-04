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
    // Prepare some stuff (output filename and ffmpeg arguments...)
    prepare();

    // Skip file if it has already been converted
    if(Settings::SkipExistingFiles) {
        QFileInfo outfileinfo(outfile);
        if(outfileinfo.exists()) {
            // File already exists, no need to convert it again
            emit ConvertDone(id, ConvertStatus::Skipped);
            return;
        }
    }

    qDebug() << "Starting convert job" << id << "|" << infile << "->" << outfile;

    // Create output dir if it does not exist
    QDir().mkpath(outdir);

    // Create FFmpeg process
    QProcess *ffmpeg = new QProcess();
    ffmpeg->setProgram(Settings::FFmpegBinary);
    ffmpeg->setArguments(ffmpegArgs);
    // Run FFmpeg process
    ffmpeg->start();
    ffmpeg->waitForFinished();
    int exitCode = ffmpeg->exitCode();

    qDebug() << "Finished job" << id << "with exit code" << exitCode;

    bool success = (exitCode == 0);

    if(success) {
        emit ConvertDone(id, ConvertStatus::Done);
    } else {
        emit ConvertDone(id, ConvertStatus::Failed);
    }
}

void FFmpegTask::prepare() {
    QFileInfo fileInfo(infile);
    QDir qdir = fileInfo.absoluteDir();
    QString dir = qdir.path();
    QString name = fileInfo.fileName();
    QString basename = name.left(name.lastIndexOf("."));
    if (!dir.startsWith(QDir::separator())) dir = QDir::separator() + dir;  // add dir separator if missing
#ifdef Q_OS_WIN
    outdir = outdir.replace(":", "");  // Fix path on Windows
#endif
    outdir = (outdir + dir);  // add dir path of the input file to the outdir to reconstruct directory structure
    outfile = outdir + QDir::separator() + basename;  // output file path without extension (will be added later)

    ffmpegArgs << "-hide_banner";
    if(Settings::SkipExistingFiles) {
        ffmpegArgs<< "-n";  // Not overwrite (keep existing file)
    } else {
        ffmpegArgs<< "-y";  // Overwrite (reconvert if file exists)
    }
    ffmpegArgs << "-i" << infile;

    if(Settings::OutputFormat == "mp3") {
        outfile += ".mp3";
        // mp3 options
        ffmpegArgs << "-c:a" << "libmp3lame";
        if(Settings::Quality == "Extreme") {
            ffmpegArgs << "-b:a" << "320k";
        } else if(Settings::Quality == "High") {
            ffmpegArgs << "-q:a" << "1";
        } else if(Settings::Quality == "Medium") {
            ffmpegArgs << "-q:a" << "4";
        }
        if (Settings::ChangeSamplerate) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";
        ffmpegArgs << "-id3v2_version" << "3";

    } else if(Settings::OutputFormat == "ogg") {
        outfile += ".ogg";
        // ogg options
        ffmpegArgs << "-vn"; //TODO: remove this to keep album art but without the output is a video, will have to find a solution
        ffmpegArgs << "-c:a" << "libvorbis";
        if(Settings::Quality == "Extreme") {
            ffmpegArgs << "-q:a" << "9";
        } else if(Settings::Quality == "High") {
            ffmpegArgs << "-q:a" << "6";
        } else if(Settings::Quality == "Medium") {
            ffmpegArgs << "-q:a" << "4";
        }
        if (Settings::ChangeSamplerate) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "flac") {
        outfile += ".flac";
        // flac options
        ffmpegArgs << "-c:a" << "flac";
        if (Settings::Quality == "Medium") ffmpegArgs << "-sample_fmt" << "s16";
        if (Settings::ChangeSamplerate) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "wav") {
        outfile += ".wav";
        // wav options
        ffmpegArgs << "-c:a" << "pcm_s16le";
        if (Settings::ChangeSamplerate) ffmpegArgs << "-ar" << Settings::OutputSamplerate;

    } else {
        // unknown format options
        outfile += "." + Settings::OutputFormat;
    }

    ffmpegArgs << outfile;
}
