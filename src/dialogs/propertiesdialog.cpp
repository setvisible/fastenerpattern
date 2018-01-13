/* - FastenerPattern - Copyright (C) 2016 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

#include <Core/Calculator>

PropertiesDialog::PropertiesDialog(Calculator *calculator, QWidget *parent) : QDialog(parent)
  , ui(new Ui::PropertiesDialog)
  , m_calculator(calculator)
{
    ui->setupUi(this);

    Q_ASSERT(m_calculator);
    ui->titleLineEdit->setText(m_calculator->title());
    ui->authorLineEdit->setText(m_calculator->author());
    ui->dateLineEdit->setText(m_calculator->date());
    ui->descriptionTextEdit->setPlainText(m_calculator->description());
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

void PropertiesDialog::accept()
{
    Q_ASSERT(m_calculator);
    m_calculator->setTitle(ui->titleLineEdit->text());
    m_calculator->setAuthor(ui->authorLineEdit->text());
    m_calculator->setDate(ui->dateLineEdit->text());
    m_calculator->setDescription(ui->descriptionTextEdit->toPlainText());

    QDialog::accept();
}
