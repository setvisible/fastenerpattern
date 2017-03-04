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

#include "solverwidget.h"
#include "ui_solverwidget.h"

#include <QtWidgets/QButtonGroup>

SolverWidget::SolverWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::SolverWidget)
  , m_params(SolverParameters::NoSolver)
{
    ui->setupUi(this);

    QButtonGroup *maingroup = new QButtonGroup(this);
    maingroup->addButton(ui->radioSolver_0);
    maingroup->addButton(ui->radioSolver_1);
    maingroup->addButton(ui->radioSolver_2);

    QButtonGroup *RBSgroup = new QButtonGroup(this);
    RBSgroup->addButton(ui->radioRBS_1);
    RBSgroup->addButton(ui->radioRBS_2);

    QObject::connect(ui->radioSolver_0, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));
    QObject::connect(ui->radioSolver_1, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));
    QObject::connect(ui->radioSolver_2, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));
    QObject::connect(ui->radioRBS_1, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));
    QObject::connect(ui->radioRBS_2, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));

    setParams(SolverParameters::RigidBodySolverWithIsoBearing);

    /* tempo */
    ui->radioSolver_2->setEnabled(false);
}

SolverWidget::~SolverWidget()
{
    delete ui;
}


SolverParameters SolverWidget::params() const
{
    return m_params;
}

void SolverWidget::setParams(SolverParameters params)
{
    if (m_params == params)
        return;

    ui->radioRBS_1->setEnabled(ui->radioSolver_1->isChecked());
    ui->radioRBS_2->setEnabled(ui->radioSolver_1->isChecked());

    toGUI(params);
    m_params = params;

    Q_ASSERT(m_params == params); /* This is not trivial because toGUI() re-entres setParams(). */
    emit paramsChanged(m_params);
}

void SolverWidget::onRadioToggled()
{
    SolverParameters params = fromGUI();
    setParams(params);
}

void SolverWidget::toGUI(SolverParameters params)
{
    switch (params) {
    case SolverParameters::NoSolver:
        ui->radioSolver_0->setChecked(true);
        break;
    case SolverParameters::RigidBodySolverWithIsoBearing:
        ui->radioSolver_1->setChecked(true);
        ui->radioRBS_1->setChecked(true);
        break;
    case SolverParameters::RigidBodySolverWithIsoShear:
        ui->radioSolver_1->setChecked(true);
        ui->radioRBS_2->setChecked(true);
        break;
    case SolverParameters::FiniteElementSolver:
        ui->radioSolver_2->setChecked(true);
        break;
    default:
        break;
    }
}

SolverParameters SolverWidget::fromGUI() const
{
    if (ui->radioSolver_0->isChecked()) {
        return SolverParameters::NoSolver;
    } else if (ui->radioSolver_1->isChecked()) {
        if(ui->radioRBS_1->isChecked()) {
            return SolverParameters::RigidBodySolverWithIsoBearing;
        }else if(ui->radioRBS_2->isChecked()) {
            return SolverParameters::RigidBodySolverWithIsoShear;
        }
    } else if (ui->radioSolver_2->isChecked()) {
        return SolverParameters::FiniteElementSolver;
    }
    return SolverParameters::NoSolver;
}
