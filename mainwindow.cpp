#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString version = "0.2";
    setWindowTitle(windowTitle() + " v" + version);

    //TODO: let the user set this in GUI
    outdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir().separator() + "FFaudioConverter";

    // Init tableView related stuff
    model = new QStandardItemModel(this);

    model->setColumnCount(2);
    model->setHorizontalHeaderLabels(QStringList({"Status", "File"}));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    setAcceptDrops(true);

    // Init own thread pool
    threadpool_converts = new QThreadPool();
    // Set max parallel threads
    //TODO: let the user set this in GUI
    int threadCount = QThread::idealThreadCount();
    qDebug() << "Using threads:" << threadCount;
    threadpool_converts->setMaxThreadCount(threadCount);
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


void MainWindow::updateTableValue(int row, int column, QString content)
{
    QModelIndex index = model->index(row, column, QModelIndex());
    model->setData(index, content);
}

void MainWindow::setRowStatusBackground(int row, QColor color)
{
    QModelIndex index = model->index(row, 0, QModelIndex()); // Status is column 0
    model->setData(index, color, Qt::BackgroundRole);
}

void MainWindow::addRow()
{
    model->insertRows(model->rowCount(), 1);
}

void MainWindow::resetTableModel()
{
    model->removeRows(0, model->rowCount());
}

void MainWindow::on_pushButton_Clear_clicked()
{
    resetTableModel();
}

void MainWindow::addFile(QString file) {
    addRow();
    int rowIndex = model->rowCount()-1;
    updateTableValue(rowIndex, 1, file);
    setRowStatusBackground(rowIndex, QColor(Qt::yellow));
}

void MainWindow::addDirectory(QString dir) {
    QList<QString> files = DirScan::ListFiles(dir);
    for(int i = 0; i < files.count(); i++)
    {
        addFile(files[i]);
    }
}

void MainWindow::on_pushButton_Convert_clicked()
{
    convert_itemCount = model->rowCount();

    if(!(convert_itemCount > 0)) {
        return;
    }

    setButtonsEnabled(false);

    for(int i = 0; i < convert_itemCount; ++i)
    {
        QModelIndex modelIndex = model->index(i, 1);
        QVariant name = model->data(modelIndex);
        QString value = name.toString();
        int index = (i + 1);

        convertItem(index, value);
    }
}

void MainWindow::convertItem(int id, QString path)
{
    FFmpegTask *task = new FFmpegTask(id, path, outdir);
    task->setAutoDelete(true);
    connect(task, SIGNAL(Done(int, bool)), this, SLOT(onConvertDone(int, bool)), Qt::QueuedConnection);
    threadpool_converts->start(task);
}

void MainWindow::onConvertDone(int id, bool success)
{
    int rowIndex = (id - 1);
    if(success) {
        setRowStatusBackground(rowIndex, QColor(Qt::green));
    } else {
        setRowStatusBackground(rowIndex, QColor(Qt::red));
    }

    // Check if the was the last file
    if(id == convert_itemCount) {
        // Convert of all files is done
        setButtonsEnabled(true);
    }
}

void MainWindow::setButtonsEnabled(bool e) {
    ui->pushButton_Convert->setEnabled(e);
    ui->pushButton_Settings->setEnabled(e);
    ui->pushButton_Clear->setEnabled(e);
}

void MainWindow::on_pushButton_Settings_clicked()
{
    SettingsDialog *settingsDialog = new SettingsDialog();
    settingsDialog->setModal(true);
    settingsDialog->setVisible(true);
}
