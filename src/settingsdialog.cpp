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

    ui->comboBox_Quality->addItem("Extreme");
    ui->comboBox_Quality->addItem("High");
    ui->comboBox_Quality->addItem("Medium");

    ui->comboBox_OutputSamplerate->addItem("44100");
    ui->comboBox_OutputSamplerate->addItem("48000");

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
    Settings::FFmpegBinary = ui->lineEdit_FFmpegBinary->text().trimmed();
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentText();
    Settings::Quality = ui->comboBox_Quality->currentText();
    Settings::OutputDirectory = ui->lineEdit_OutputDirectory->text().trimmed();
    Settings::ChangeSamplerate = ui->checkBox_ChangeSamplerate->isChecked();
    Settings::OutputSamplerate = ui->comboBox_OutputSamplerate->currentText();
    Settings::SkipExistingFiles = ui->checkBox_SkipExistingFiles->isChecked();
    Settings::Multithreading = ui->checkBox_Multithreading->isChecked();

    QSettings settings;

    settings.setValue("FFmpegBinary", Settings::FFmpegBinary);
    settings.setValue("OutputFormat", Settings::OutputFormat);
    settings.setValue("Quality", Settings::Quality);
    settings.setValue("OutputDirectory", Settings::OutputDirectory);
    settings.setValue("ChangeSamplerate", Settings::ChangeSamplerate);
    settings.setValue("OutputSamplerate", Settings::OutputSamplerate);
    settings.setValue("SkipExistingFiles", Settings::SkipExistingFiles);
    settings.setValue("Multithreading", Settings::Multithreading);

    qDebug() << "Settings saved";
}

void SettingsDialog::LoadSettings()
{
    ui->lineEdit_FFmpegBinary->setText(Settings::FFmpegBinary);
    ui->comboBox_OutputFormat->setCurrentText(Settings::OutputFormat);
    ui->comboBox_Quality->setCurrentText(Settings::Quality);
    ui->lineEdit_OutputDirectory->setText(Settings::OutputDirectory);
    ui->checkBox_ChangeSamplerate->setChecked(Settings::ChangeSamplerate);
    ui->comboBox_OutputSamplerate->setCurrentText(Settings::OutputSamplerate);
    ui->checkBox_SkipExistingFiles->setChecked(Settings::SkipExistingFiles);
    ui->checkBox_Multithreading->setChecked(Settings::Multithreading);
}