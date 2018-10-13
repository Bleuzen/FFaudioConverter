#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Init settings elements
    ui->comboBox_OutputFormat->addItem("mp3");
    ui->comboBox_OutputFormat->addItem("ogg");
    ui->comboBox_OutputFormat->addItem("wav");

    // Set settings in GUI
    ui->comboBox_OutputFormat->setCurrentText(Settings::OutputFormat);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    // Apply settings
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentText();
}
