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

#include "fastenerwidget.h"
#include "ui_fastenerwidget.h"

FastenerWidget::FastenerWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::FastenerWidget)
  , m_currentIndex(-1)
{
    ui->setupUi(this);

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
void FastenerWidget::onTextChanged(QString /*text*/)
{
    this->onChanged();
}
void FastenerWidget::onValueChanged(double /*value*/)
{
    this->onChanged();
}
void FastenerWidget::onStateChanged(int /*value*/)
{
    this->onChanged();
}

void FastenerWidget::onChanged()
{
    Fastener fastener = this->fastener();
    model()->setFastener(m_currentIndex, fastener);
}

/******************************************************************************
 ******************************************************************************/
void FastenerWidget::onFastenerChanged(const int index, const Fastener &fastener)
{
    /// \todo Add the other slots (fastenerRemoved and fastenerAdded) ?

    Q_UNUSED(index);
    Q_UNUSED(fastener);
    /// \todo use these arguments ?

    if (m_currentIndex >= 0 && m_currentIndex < model()->fastenerCount()) {
        Fastener fastener = model()->fastenerAt(m_currentIndex);
        this->setFastener(fastener);
    }
}

void FastenerWidget::onSelectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
    if (set.count() == 1) {
        QSetIterator<int> it(set);
        m_currentIndex = it.next();
        Fastener fastener = model()->fastenerAt(m_currentIndex);
        this->setFastener(fastener);
    } else {
        m_currentIndex = -1;
    }
    ui->groupBox->setEnabled(set.count() == 1);
}

/******************************************************************************
 ******************************************************************************/
Fastener FastenerWidget::fastener() const
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

void FastenerWidget::setFastener(const Fastener &fastener)
{
    bool blocked = this->signalsBlocked();
    this->blockSignals(true);
    ui->nameEdit->setText( fastener.name );
    ui->positionX->setValue( fastener.positionX.value() *1000); // mm !
    ui->positionY->setValue( fastener.positionY.value() *1000); // mm !
    ui->thickness->setValue( fastener.thickness.value() *1000); // mm !
    ui->diameter->setValue(  fastener.diameter.value() *1000);  // mm !
    ui->dof_X->setChecked( Fastener::DOFtoBool( fastener.DoF_X ));
    ui->dof_Y->setChecked( Fastener::DOFtoBool( fastener.DoF_Y ));
    this->blockSignals(blocked);

    model()->setFastener(m_currentIndex, fastener);
}
