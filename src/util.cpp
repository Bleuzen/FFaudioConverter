/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018-2019  Bleuzen
https://github.com/Bleuzen/FFaudioConverter
supgesu@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------- */

#include "util.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>
#include <QProcess>

#include "settings.h"

bool Util::isBinary(QString file) {
    if (file.length() <= 0) return false;
    QFileInfo fileInfo(file);
    return (fileInfo.exists() && fileInfo.isFile() && fileInfo.isExecutable());
}

bool Util::prepareAndCheckDirectory(QString dir) {
    QFileInfo dirInfo(dir);
    // Create dir if it does not exist
    if(!dirInfo.exists()) {
        QDir().mkpath(dir);
    }
    // Return if dir could be created and is writable
    return (dirInfo.exists() && dirInfo.isDir() && dirInfo.isWritable());
}

bool Util::isNullOrEmpty(QString string) {
    return string.isNull() || string.isEmpty();
}

bool Util::mayBeAudioOrVideoFile(QString file) {
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(file);
    QString type = mime.name().split("/")[0];
    if(isNullOrEmpty(type) || type == "audio" || type == "video") {
        return true;
    }
    return false;
}

/*bool Util::ffmpegBinarySupportsOutputFormat() {
    QProcess *ffmpeg = new QProcess();
    ffmpeg->setProgram(Settings::FFmpegBinary);
    ffmpeg->setArguments(QStringList() << "-hide_banner" << "-encoders");
    ffmpeg->start();
    ffmpeg->waitForFinished();
    int exitCode = ffmpeg->exitCode();

    if(exitCode != 0) return false;

    QString output(ffmpeg->readAllStandardOutput());

    if(Settings::OutputFormat == "mp3") {
        return output.contains("libmp3lame");

    } else if(Settings::OutputFormat == "m4a") {
        return output.contains("aac");

    } else if(Settings::OutputFormat == "ogg") {
        return output.contains("libvorbis");

    } else if(Settings::OutputFormat == "opus") {
        return output.contains("libopus");

    } else if(Settings::OutputFormat == "flac") {
        return output.contains("flac");

    } else if(Settings::OutputFormat == "wav") {
        return output.contains("pcm_s16le");

    } else {
        return false;
    }
}*/
