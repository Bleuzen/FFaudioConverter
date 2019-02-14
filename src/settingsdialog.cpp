/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018-2019  Elias Martin (Bleuzen)
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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>

#include "util.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Disable resize
    setFixedSize(width(), height());

    // Init settings elements
    ui->comboBox_OutputFormat->addItem("MP3 (.mp3)", "mp3");
    ui->comboBox_OutputFormat->addItem("AAC (.m4a)", "m4a");
    ui->comboBox_OutputFormat->addItem("Ogg Vorbis (.ogg)", "ogg");
    ui->comboBox_OutputFormat->addItem("Opus (.opus)", "opus");
    ui->comboBox_OutputFormat->addItem("FLAC (.flac)", "flac");
    ui->comboBox_OutputFormat->addItem("WAV (.wav)", "wav");

    ui->comboBox_Quality->addItem(tr("Medium"), "medium");
    ui->comboBox_Quality->addItem(tr("High"), "high");
    ui->comboBox_Quality->addItem(tr("Extreme"), "extreme");
    ui->comboBox_Quality->addItem(tr("Custom"), "custom");

    ui->comboBox_OutputSamplerate->addItem(tr("Keep"), "");
    ui->comboBox_OutputSamplerate->addItem("44100 Hz", "44100");
    ui->comboBox_OutputSamplerate->addItem("48000 Hz", "48000");
    ui->comboBox_OutputSamplerate->addItem("96000 Hz", "96000");

    ui->comboBox_AudioFiltersPresets->addItem(tr("Disabled"), "");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Increase volume"), "volume=10dB");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Reduce volume"), "volume=-5dB");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Fade-in and fade-out"), "afade=duration=5\nareverse\nafade=duration=5\nareverse");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Bass boost"), "volume=-10dB\nbass=gain=10");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Speed up"), "atempo=1.5");
    ui->comboBox_AudioFiltersPresets->setCurrentIndex(-1);

    // Set settings in GUI
    LoadSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    ApplySettings();
}

void SettingsDialog::ApplySettings()
{
    Settings::FFmpegBinary = ui->lineEdit_FFmpegBinary->text().trimmed();
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentData().toString();
    Settings::Quality = ui->comboBox_Quality->currentData().toString();
    Settings::OutputDirectory = ui->lineEdit_OutputDirectory->text().trimmed();
    Settings::OutputSamplerate = ui->comboBox_OutputSamplerate->currentData().toString();
    Settings::AudioFilters = ui->plainTextEdit_AudioFilters->toPlainText().trimmed();
    Settings::UseSoXresampler = ui->checkBox_UseSoXresampler->isChecked();
    Settings::QuickConvertMode = ui->checkBox_QuickConvertMode->isChecked();
    Settings::Threads = ui->spinBox_Threads->value();

    Settings::save();
}

void SettingsDialog::LoadSettings()
{
    ui->lineEdit_FFmpegBinary->setText(Settings::FFmpegBinary);
    ui->comboBox_OutputFormat->setCurrentIndex(ui->comboBox_OutputFormat->findData(Settings::OutputFormat));
    ui->comboBox_Quality->setCurrentIndex(ui->comboBox_Quality->findData(Settings::Quality));
    ui->lineEdit_OutputDirectory->setText(Settings::OutputDirectory);
    ui->comboBox_OutputSamplerate->setCurrentIndex(ui->comboBox_OutputSamplerate->findData(Settings::OutputSamplerate));
    ui->plainTextEdit_AudioFilters->setPlainText(Settings::AudioFilters);
    ui->checkBox_UseSoXresampler->setChecked(Settings::UseSoXresampler);
    ui->checkBox_QuickConvertMode->setChecked(Settings::QuickConvertMode);
    ui->spinBox_Threads->setValue(Settings::Threads);
}

void SettingsDialog::on_toolButton_SelectOutputDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), Settings::OutputDirectory, QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(!Util::isNullOrEmpty(dir)) {
        ui->lineEdit_OutputDirectory->setText(dir);
    }
}

void SettingsDialog::on_comboBox_AudioFiltersPresets_activated(int index)
{
    ui->plainTextEdit_AudioFilters->setPlainText(ui->comboBox_AudioFiltersPresets->itemData(index).toString());
}

void SettingsDialog::on_comboBox_Quality_activated(int index)
{
    // Show the Quality tab if custom quality is selected or hide it if a preset is used
    if(index == ui->comboBox_Quality->findData("custom")) {
        QString format = ui->comboBox_OutputFormat->currentData().toString();
        QString title = ui->label_Quality->text().replace(":", "");

        if(format == "wav") {
            QStringList bitdepths;  //TODO: rename?
            bitdepths << "16" << "24" << "32";
            bool ok;
            QString item = QInputDialog::getItem(this, title, tr("Bit depth:"), bitdepths, bitdepths.indexOf(Settings::CustomQualityArguments), false, &ok);
            if(ok && !item.isEmpty()) {
                Settings::CustomQualityArguments = item;
            }
        } else {
            bool ok;
            QString text = QInputDialog::getText(this, title, tr("FFmpeg arguments:"), QLineEdit::Normal,
                                                 Settings::CustomQualityArguments.isEmpty() ? "" : Settings::CustomQualityArguments,
                                                 &ok);
            if(ok && !text.isEmpty()) {
                Settings::CustomQualityArguments = text;
            } else {
                ui->comboBox_Quality->setCurrentIndex(ui->comboBox_Quality->findData(Settings::Quality));
            }
        }

    } else {
        Settings::CustomQualityArguments = "";
    }
}

void SettingsDialog::on_comboBox_OutputFormat_activated(int /* unused */)
{
    Settings::CustomQualityArguments = "";
    if(ui->comboBox_Quality->currentData().toString() == "custom") {
        ui->comboBox_Quality->setCurrentIndex(ui->comboBox_Quality->findData("high"));
    }
}
