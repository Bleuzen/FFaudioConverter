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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // Disable resize
    setFixedSize(width(), height());

    ui->label_Appname->setText(QCoreApplication::applicationName());
    ui->label_Version->setText("v" + QCoreApplication::applicationVersion());
    ui->label_Developer->setText(tr("developed by") + " Elias Martin (" + QCoreApplication::organizationName() + ")");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_Close_clicked()
{
    this->close();
}

void AboutDialog::on_pushButton_AboutQt_clicked()
{
    QMessageBox::aboutQt(this);
}
