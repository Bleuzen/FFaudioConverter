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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QMenu>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QCoreApplication::applicationName());

    // Init tableView related stuff
    QStringList tableColumns = QStringList({tr("Status"), tr("File")});
    model = new QStandardItemModel(this);
    model->setColumnCount(tableColumns.size());
    model->setHorizontalHeaderLabels(tableColumns);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Set selection properties for tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Use custom context menu for tableView
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // We are not converting right now (hide progressbar and disable cancel button)
    setIsConverting(false);

    // Allow to drop input files in this window
    setAcceptDrops(true);

    // Hide tableView at the beginning (only display help message)
    ui->tableView->setVisible(false);

    // Init own thread pool
    threadpool_converts = new QThreadPool();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (isConverting) return;  // Do not accept drops during convert is running

    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    // Display tableView if it is still hidden
    if(!ui->tableView->isVisible()) {
        ui->tableView->setVisible(true);
        // Hide help message
        ui->label_HelpMessage->setVisible(false);
    }

    // Handle drop
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    int droppedUrlCnt = droppedUrls.size();
    for(int i = 0; i < droppedUrlCnt; i++) {
        QString localPath = droppedUrls[i].toLocalFile();
        QFileInfo fileInfo(localPath);
        if(fileInfo.isFile()) {
// file
            QString file = fileInfo.absoluteFilePath();
            qDebug() << "Dropped file:" << file;
            addFile(file);
        }
        else if(fileInfo.isDir()) {
// directory
            QString dir = fileInfo.absoluteFilePath();
            qDebug() << "Dropped dir:" << dir;
            addDirectory(dir);
        }
        else {
// none
        }
    }
    event->acceptProposedAction();
}

void MainWindow::addFile(QString file) {
    insertTableRow();
    int row = model->rowCount()-1;
    setItemFileValue(row, file);
    setConvertItemStatus(row, FFmpegTask::ConvertStatus::Pending);
}

void MainWindow::addDirectory(QString dir) {
    QList<QString> files = DirScan::ListFiles(dir);
    for(int i = 0; i < files.count(); i++)
    {
        addFile(files[i]);
    }
}

QModelIndex MainWindow::rowToStatusIndex(int row) {
    QModelIndex index = model->index(row, 0, QModelIndex());  // Status is in column 0
    return index;
}

QModelIndex MainWindow::rowToFileIndex(int row) {
    QModelIndex index = model->index(row, 1, QModelIndex());  // File is in column 0
    return index;
}

void MainWindow::insertTableRow()
{
    model->insertRows(model->rowCount(), 1);
}

void MainWindow::setItemFileValue(int row, QString content)
{
    QModelIndex index = rowToFileIndex(row);
    model->setData(index, content);
}

void MainWindow::setConvertItemStatus(int row, FFmpegTask::ConvertStatus status)
{
    QModelIndex index = rowToStatusIndex(row);
    setConvertItemStatus(index, status);
}

void MainWindow::setConvertItemStatus(QModelIndex index, FFmpegTask::ConvertStatus status)
{
    QColor sColor;

    if(status == FFmpegTask::ConvertStatus::Pending) {
        model->setData(index, QColor(Qt::yellow), Qt::BackgroundRole);
    } else if(status == FFmpegTask::ConvertStatus::Done) {
        model->setData(index, QColor(Qt::green), Qt::BackgroundRole);
    } else if(status == FFmpegTask::ConvertStatus::Failed) {
        model->setData(index, QColor(Qt::red), Qt::BackgroundRole);
    } else if(status == FFmpegTask::ConvertStatus::Skipped) {
        model->setData(index, QColor(Qt::blue), Qt::BackgroundRole);
    }
}

void MainWindow::removeTableRows(QList<int> rows)
{
    int prev = -1;
    for (int i = rows.count() - 1; i >= 0; i--) {
       int current = rows[i];
       if(current != prev) {
          model->removeRow(current);
          prev = current;
       }
    }
}

void MainWindow::resetTableModel()
{
    model->removeRows(0, model->rowCount());
}

void MainWindow::convertItem(int id, QString file)
{
    FFmpegTask *task = new FFmpegTask(id, file, Settings::OutputDirectory);
    task->setAutoDelete(true);
    connect(task, SIGNAL(ConvertDone(int, FFmpegTask::ConvertStatus)), this, SLOT(onConvertDone(int, FFmpegTask::ConvertStatus)), Qt::QueuedConnection);
    threadpool_converts->start(task);
}

void MainWindow::cancel() {
    ui->pushButton_Cancel->setEnabled(false);  // Prevent the user from double-clicking Cancel

    threadpool_converts->clear();  // Clear pending converts
    QFuture<void> future = QtConcurrent::run([=]() {
        threadpool_converts->waitForDone();  // Wait until all currently running converts are done
        setIsConverting(false);  // Enable the buttons again to be able to start a new convert
    });
}

void MainWindow::onConvertDone(int id, FFmpegTask::ConvertStatus status)
{
    convert_doneItemsCount++;

    int row = (id - 1);
    QModelIndex index = rowToStatusIndex(row);

    // Update status
    setConvertItemStatus(index, status);

    // Scroll to last item which is done (if no other previous item is still converting)
    // Disabled for now because it can behave weird in some cases and some users may not want it.
    //TODO: Maybe implement a setting for this?
//    if(id <= convert_doneItemsCount) {
//        ui->tableView->scrollTo(index);
//    }

    // Update progress and progressbar
    convert_progress = (convert_doneItemsCount * 100 / convert_totalItemsCount);
    ui->progressBar->setValue(convert_progress);

    // Check if the was the last file
    if(convert_doneItemsCount == convert_totalItemsCount) {
        // Convert of all files is done
        qDebug() << "Done";
        setIsConverting(false);
    }
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if (isConverting) return;  // Do not show Remove option during converting

    //QModelIndex selectedRow = ui->tableView->indexAt(pos);  // single selected row
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();  // all selected rows

    QMenu *menu = new QMenu(this);

    QAction *action_Remove = new QAction("Remove", this);
    menu->addAction(action_Remove);
    connect(action_Remove, &QAction::triggered, this, [=]() {
        // Collect all selected rows
        QList<int> rows;
        foreach (QModelIndex index, selectedRows) {
            rows.append(index.row());
        }

        // Remove all selected rows
        removeTableRows(rows);
    });

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::setIsConverting(bool e) {
    isConverting = e;

    ui->pushButton_Convert->setEnabled(!e);
    ui->pushButton_Cancel->setEnabled(e);
    ui->pushButton_Settings->setEnabled(!e);
    ui->pushButton_Clear->setEnabled(!e);

    ui->progressBar->setVisible(e);
}

void MainWindow::on_pushButton_Convert_clicked()
{
    // Check settings
    if(!Util::isBinary(Settings::FFmpegBinary)) {
        QMessageBox::warning(this, "Error", "FFmpeg binary is invalid. Please check your settings.");
        return;
    }
    if(!Util::prepareAndCheckDirectory(Settings::OutputDirectory)) {
        QMessageBox::warning(this, "Error", "Your output directory is bad. Maybe it is not writable?");
        return;
    }

    convert_totalItemsCount = model->rowCount();
    convert_doneItemsCount = 0;
    ui->progressBar->setValue(0);

    if(!(convert_totalItemsCount > 0)) {
        return;
    }

    int threadCount = Settings::Multithreading ? QThread::idealThreadCount() : 1;
    threadpool_converts->setMaxThreadCount(threadCount);

    qDebug() << "Starting convert..." << "\n"
             << "Threads:" << threadCount << "\n"
             << "Items:" << convert_totalItemsCount;

    setIsConverting(true);

    for(int row = 0; row < convert_totalItemsCount; ++row)
    {
        QModelIndex modelIndex = rowToFileIndex(row);
        QVariant name = model->data(modelIndex);
        QString file = name.toString();
        int id = (row + 1);

        convertItem(id, file);
    }
}

void MainWindow::on_pushButton_Cancel_clicked()
{
    cancel();
}

void MainWindow::on_pushButton_Clear_clicked()
{
    resetTableModel();
}

void MainWindow::on_pushButton_Settings_clicked()
{
    SettingsDialog *settingsDialog = new SettingsDialog();
    settingsDialog->setModal(true);
    settingsDialog->setVisible(true);
}

void MainWindow::on_pushButton_About_clicked()
{
    QMessageBox msgBox;
    QPushButton *sourceCodeButton = msgBox.addButton("Source code", QMessageBox::ActionRole);
    QPushButton *aboutQtButton = msgBox.addButton("About Qt", QMessageBox::ActionRole);
    QPushButton *closeButton = msgBox.addButton(QMessageBox::Close);

    msgBox.setText("GUI for converting audio files with FFmpeg\n"
                   "Version: " + QCoreApplication::applicationVersion());

    msgBox.exec();

    if(msgBox.clickedButton() == sourceCodeButton) {
        QDesktopServices::openUrl(QUrl("https://github.com/Bleuzen/FFaudioConverter"));
    } else if(msgBox.clickedButton() == aboutQtButton) {
        QMessageBox::aboutQt(this);
    } else if(msgBox.clickedButton() == closeButton) {
        // Do nothing, just close
    }
}
