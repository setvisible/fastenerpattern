/* - FastenerPattern - Copyright (C) 2016-2018 Sebastien Vavassori
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

#include "fastenerwidget.h"
#include "ui_fastenerwidget.h"

#include <Core/AbstractSpliceModel>

#include <QtCore/QTimer>

FastenerWidget::FastenerWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::FastenerWidget)
  , m_currentIndex(-1)
  , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInfo()));

    QObject::connect(ui->nameEdit , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
    QObject::connect(ui->positionX, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->positionY, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->thickness, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->diameter , SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    QObject::connect(ui->dof_X    , SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
    QObject::connect(ui->dof_Y    , SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

}

FastenerWidget::~FastenerWidget()
{
    delete ui;
}

int FastenerWidget::decimals() const
{
    return ui->positionX->decimals();
}

void FastenerWidget::setDecimals(int prec)
{
    ui->positionX->setDecimals(prec);
    ui->positionY->setDecimals(prec);
    ui->thickness->setDecimals(prec);
    ui->diameter->setDecimals(prec);
}


/******************************************************************************
 ******************************************************************************/
void FastenerWidget::onTextChanged(QString)
{
    onChanged();
}

void FastenerWidget::onValueChanged(double)
{
    onChanged();
}

void FastenerWidget::onStateChanged(int)
{
    onChanged();
}

void FastenerWidget::onChanged()
{
    Fastener fastener = fromGUI();
    model()->setFastener(m_currentIndex, fastener);
}

/******************************************************************************
 ******************************************************************************/
void FastenerWidget::onFastenerChanged(const int, const Fastener &)
{
    updateInfoLater(C_SHORT_DELAY_MSEC);
}

void FastenerWidget::onSelectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
    if (set.count() == 1) {
        QSetIterator<int> it(set);
        m_currentIndex = it.next();
        Fastener fastener = model()->fastenerAt(m_currentIndex);
        toGUI(fastener);
        ui->groupBox->setEnabled(true);
    } else {
        if (ui->groupBox->isEnabled()) {
            ui->groupBox->setEnabled(false);
            m_currentIndex = -1;
            Fastener dummyfastener;
            dummyfastener.name = tr("<Select a fastener>");
            toGUI(dummyfastener);
        }
    }
}

/******************************************************************************
 ******************************************************************************/
void FastenerWidget::updateInfoLater(int msec)
{
    m_timer->stop();
    m_timer->start(msec);
}

void FastenerWidget::updateInfo()
{
    m_timer->stop();

    if (m_currentIndex >= 0 && m_currentIndex < model()->fastenerCount()) {
        Fastener fastener = model()->fastenerAt(m_currentIndex);
        toGUI(fastener);
    }
}

/******************************************************************************
 ******************************************************************************/
Fastener FastenerWidget::fromGUI() const
{
    Fastener fastener;
    fastener.name = ui->nameEdit->text();
    fastener.positionX = ui->positionX->value() *_mm;
    fastener.positionY = ui->positionY->value() *_mm;
    fastener.thickness = ui->thickness->value() *_mm;
    fastener.diameter = ui->diameter->value() *_mm;
    fastener.DoF_X = Fastener::boolToDOF( ui->dof_X->isChecked() );
    fastener.DoF_Y = Fastener::boolToDOF( ui->dof_Y->isChecked() );
    return fastener;
}

void FastenerWidget::toGUI(const Fastener &fastener)
{
    bool blocked_nameEdit  = ui->nameEdit->blockSignals(true);
    bool blocked_positionX = ui->positionX->blockSignals(true);
    bool blocked_positionY = ui->positionY->blockSignals(true);
    bool blocked_thickness = ui->thickness->blockSignals(true);
    bool blocked_diameter  = ui->diameter->blockSignals(true);
    bool blocked_dof_X     = ui->dof_X->blockSignals(true);
    bool blocked_dof_Y     = ui->dof_Y->blockSignals(true);

    ui->nameEdit->setText( fastener.name );
    ui->positionX->setValue( fastener.positionX.value() *1000); // mm !
    ui->positionY->setValue( fastener.positionY.value() *1000); // mm !
    ui->thickness->setValue( fastener.thickness.value() *1000); // mm !
    ui->diameter->setValue(  fastener.diameter.value() *1000);  // mm !
    ui->dof_X->setChecked( Fastener::DOFtoBool( fastener.DoF_X ));
    ui->dof_Y->setChecked( Fastener::DOFtoBool( fastener.DoF_Y ));

    ui->nameEdit->blockSignals(blocked_nameEdit);
    ui->positionX->blockSignals(blocked_positionX);
    ui->positionY->blockSignals(blocked_positionY);
    ui->thickness->blockSignals(blocked_thickness);
    ui->diameter->blockSignals(blocked_diameter);
    ui->dof_X->blockSignals(blocked_dof_X);
    ui->dof_Y->blockSignals(blocked_dof_Y);
}
