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
    LoadSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    SaveSettings();
}

void SettingsDialog::SaveSettings()
{
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentText();
    Settings::OutputDirectory = ui->lineEdit_OutputDirectory->text().trimmed();
    Settings::ChangeSamplerate = ui->checkBox_ChangeSamplerate->isChecked();
    Settings::Force16bitFLAC = ui->checkBox_Force16bitFLAC->isChecked();
    Settings::Multithreading = ui->checkBox_Multithreading->isChecked();

    QSettings settings;

    settings.setValue("OutputFormat", Settings::OutputFormat);
    settings.setValue("OutputDirectory", Settings::OutputDirectory);
    settings.setValue("ChangeSamplerate", Settings::ChangeSamplerate);
    settings.setValue("Force16bitFLAC", Settings::Force16bitFLAC);
    settings.setValue("Multithreading", Settings::Multithreading);

    qDebug() << "Settings saved";
}

void SettingsDialog::LoadSettings()
{
    ui->comboBox_OutputFormat->setCurrentText(Settings::OutputFormat);
    ui->lineEdit_OutputDirectory->setText(Settings::OutputDirectory);
    ui->checkBox_ChangeSamplerate->setChecked(Settings::ChangeSamplerate);
    ui->checkBox_Force16bitFLAC->setChecked(Settings::Force16bitFLAC);
    ui->checkBox_Multithreading->setChecked(Settings::Multithreading);
}
