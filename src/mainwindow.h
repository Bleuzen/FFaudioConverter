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
    void on_pushButton_Clear_clicked();
    void on_pushButton_Convert_clicked();
    void on_pushButton_Settings_clicked();
    void on_pushButton_About_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void onConvertDone(int id, bool success);

private:
    Ui::MainWindow *ui;

    void LoadSettings();

    QStandardItemModel *model;
    QThreadPool *threadpool_converts;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void insertTableRow();
    void updateTableValue(int, int, QString);
    void setTableRowStatusBackground(int, QColor);
    void removeTableRows(QList<int>);
    void resetTableModel();

    int convert_itemCount;
    void convertItem(int, QString);

    void addFile(QString);
    void addDirectory(QString);

    void setButtonsEnabled(bool);
};

#endif // MAINWINDOW_H
