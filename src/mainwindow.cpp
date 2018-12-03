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
    model = new QStandardItemModel(this);

    model->setColumnCount(2);
    model->setHorizontalHeaderLabels(QStringList({"Status", "File"}));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Set selection properties for tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Use custom context menu for tableView
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Allow to drop input files in this window
    setAcceptDrops(true);

    // Disable tableView at the beginning (only display help message)
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
    int rowIndex = model->rowCount()-1;
    updateTableValue(rowIndex, 1, file);
    setConvertItemStatus(rowIndex, FFmpegTask::ConvertStatus::Pending);
}

void MainWindow::addDirectory(QString dir) {
    QList<QString> files = DirScan::ListFiles(dir);
    for(int i = 0; i < files.count(); i++)
    {
        addFile(files[i]);
    }
}

void MainWindow::insertTableRow()
{
    model->insertRows(model->rowCount(), 1);
}

void MainWindow::updateTableValue(int row, int column, QString content)
{
    QModelIndex index = model->index(row, column, QModelIndex());
    model->setData(index, content);
}

void MainWindow::setConvertItemStatus(int row, FFmpegTask::ConvertStatus status)
{
    QModelIndex index = model->index(row, 0, QModelIndex()); // Status is column 0
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

void MainWindow::convertItem(int id, QString path)
{
    FFmpegTask *task = new FFmpegTask(id, path, Settings::OutputDirectory);
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
    int rowIndex = (id - 1);

    setConvertItemStatus(rowIndex, status);

    // Check if the was the last file
    if(id == convert_itemCount) {
        // Convert of all files is done
        qDebug() << "Done";
        setIsConverting(false);
    }
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    // Do not show Remove option during converting
    if(isConverting) {
        return;
    }

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
}

void MainWindow::on_pushButton_Convert_clicked()
{
    convert_itemCount = model->rowCount();

    if(!(convert_itemCount > 0)) {
        return;
    }

    int threadCount = 1;
    if(Settings::Multithreading) {
        threadCount = QThread::idealThreadCount();
    }
    threadpool_converts->setMaxThreadCount(threadCount);

    qDebug() << "Starting convert with" << threadCount << "thread(s)";

    setIsConverting(true);

    for(int i = 0; i < convert_itemCount; ++i)
    {
        QModelIndex modelIndex = model->index(i, 1);
        QVariant name = model->data(modelIndex);
        QString value = name.toString();
        int index = (i + 1);

        convertItem(index, value);
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