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
