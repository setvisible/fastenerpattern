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

#include "appliedloadwidget.h"
#include "ui_appliedloadwidget.h"

#include <Core/AbstractSpliceModel>

AppliedLoadWidget::AppliedLoadWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::AppliedLoadWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->fx, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->fy, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->tz, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
}

AppliedLoadWidget::~AppliedLoadWidget()
{
    delete ui;
}

int AppliedLoadWidget::decimals() const
{
    return ui->fx->decimals();
}

void AppliedLoadWidget::setDecimals(int prec)
{
    ui->fx->setDecimals(prec);
    ui->fy->setDecimals(prec);
    ui->tz->setDecimals(prec);
}

void AppliedLoadWidget::onValueChanged(double /*value*/)
{
    Tensor lc = this->appliedLoad();
    model()->setAppliedLoad(lc);
}

void AppliedLoadWidget::onAppliedLoadChanged()
{
    Tensor lc = model()->appliedLoad();
    this->setAppliedLoad( lc );
}

Tensor AppliedLoadWidget::appliedLoad() const
{
    Tensor load;
    load.force_x  = ui->fx->value() *N;
    load.force_y  = ui->fy->value() *N;
    load.torque_z = ui->tz->value() *N_m;
    return load;
}

void AppliedLoadWidget::setAppliedLoad(const Tensor &appliedLoad)
{
    bool fx_blocked = ui->fx->blockSignals(true);
    bool fy_blocked = ui->fy->blockSignals(true);
    bool tz_blocked = ui->tz->blockSignals(true);

    ui->fx->setValue( appliedLoad.force_x.value() );
    ui->fy->setValue( appliedLoad.force_y.value() );
    ui->tz->setValue( appliedLoad.torque_z.value() );

    ui->fx->blockSignals(fx_blocked);
    ui->fy->blockSignals(fy_blocked);
    ui->tz->blockSignals(tz_blocked);

    model()->setAppliedLoad(appliedLoad);
}

