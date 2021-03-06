/* - FastenerPattern - Copyright (C) 2016-2017 Sebastien Vavassori
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

#include "designoptionwidget.h"
#include "ui_designoptionwidget.h"

#include <Core/AbstractSpliceModel>

#include <QtCore/QDebug>

DesignOptionWidget::DesignOptionWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::DesignOptionWidget)
{
    ui->setupUi(this);
}

DesignOptionWidget::~DesignOptionWidget()
{
    delete ui;
}

int DesignOptionWidget::randomIterations() const
{
    return ui->randomSpinBox->value();
}

void DesignOptionWidget::setRandomIterations(const int iterations)
{
    ui->randomSpinBox->setValue(iterations);
}

int DesignOptionWidget::localIterations() const
{
    return ui->localSpinBox->value();
}

void DesignOptionWidget::setLocalIterations(const int iterations)
{
    ui->localSpinBox->setValue(iterations);
}
