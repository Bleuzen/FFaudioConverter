#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Disable resize
    setFixedSize(width(), height());

    // Init settings elements
    ui->comboBox_OutputFormat->addItem("mp3");
    ui->comboBox_OutputFormat->addItem("ogg");
    ui->comboBox_OutputFormat->addItem("flac");
    ui->comboBox_OutputFormat->addItem("wav");

    // Set settings in GUI
    ui->comboBox_OutputFormat->setCurrentText(Settings::OutputFormat);
    ui->checkBox_ChangeSamplerate->setChecked(Settings::ChangeSamplerate);
    ui->checkBox_Force16bitFLAC->setChecked(Settings::Force16bitFLAC);
    ui->checkBox_Multithreading->setChecked(Settings::Multithreading);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    // Apply settings
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentText();
    Settings::ChangeSamplerate = ui->checkBox_ChangeSamplerate->isChecked();
    Settings::Force16bitFLAC = ui->checkBox_Force16bitFLAC->isChecked();
    Settings::Multithreading = ui->checkBox_Multithreading->isChecked();
}
