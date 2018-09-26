#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QDir>

#include "dirscan.h"
#include "ffmpegtask.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString outdir;

private slots:
    void on_pushButton_Clear_clicked();
    void on_pushButton_AddMusicDir_clicked();
    void on_pushButton_Convert_clicked();

    void onConvertDone(int id, bool success);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QThreadPool *threadpool_converts;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    int convert_itemCount;

    void updateTableValue(int, int, QString);
    void setRowStatusBackground(int, QColor);
    void addRow();
    void resetTableModel();
    void convertItem(int, QString);

    void addFile(QString);
    void addDirectory(QString);

    void setButtonsEnabled(bool);
};

#endif // MAINWINDOW_H
