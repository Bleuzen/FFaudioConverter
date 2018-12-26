/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018  Elias Martin (Bleuzen)
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QDir>
#include <QList>

#include "dirscan.h"
#include "ffmpegtask.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "util.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Convert_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_Clear_clicked();
    void on_pushButton_Settings_clicked();
    void on_pushButton_About_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void onConvertDone(int id, FFmpegTask::ConvertStatus status);

private:
    Ui::MainWindow *ui;

    QStandardItemModel *model;
    QThreadPool *threadpool_converts;

    bool isConverting;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void insertTableRow();
    void removeTableRows(QList<int>);
    void resetTableModel();

    QModelIndex rowToStatusIndex(int);
    QModelIndex rowToFileIndex(int);

    void setItemFileValue(int, QString);
    void setConvertItemStatus(int, FFmpegTask::ConvertStatus);
    void setConvertItemStatus(QModelIndex, FFmpegTask::ConvertStatus);

    int convert_totalItemsCount;
    int convert_doneItemsCount;
    int convert_progress;
    void convertItem(int, QString);
    void cancel();

    void addFile(QString);
    void addDirectory(QString);

    void setIsConverting(bool);
};

#endif // MAINWINDOW_H
