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

#include "solverwidget.h"
#include "ui_solverwidget.h"

#include <Core/AbstractSpliceModel>
#include <Core/Solvers/Parameters>

#include <QtWidgets/QButtonGroup>

SolverWidget::SolverWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::SolverWidget)
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

    QObject::connect(ui->radioSolver_1, SIGNAL(toggled(bool)),
                     ui->radioRBS_1, SLOT(setEnabled(bool)));
    QObject::connect(ui->radioSolver_1, SIGNAL(toggled(bool)),
                     ui->radioRBS_2, SLOT(setEnabled(bool)));

    toGUI(SolverParameters::RigidBodySolverWithIsoBearing);


    /// \todo implement Elastic Deformable Joint.
    // Disabled radio because 'Elastic Deformation' is not implemented yet
    ui->radioSolver_2->setEnabled(false);
}

SolverWidget::~SolverWidget()
{
    delete ui;
}

void SolverWidget::onSolverParamsChanged()
{
    SolverParameters params = model()->solverParameters();
    toGUI(params);
}

void SolverWidget::onRadioToggled()
{
    SolverParameters params = fromGUI();
    if (params != model()->solverParameters()) {
        model()->setSolverParameters(params);
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

void SolverWidget::toGUI(SolverParameters params)
{
    bool blocked_0 = ui->radioSolver_0->blockSignals(true);
    bool blocked_1 = ui->radioSolver_1->blockSignals(true);
    bool blocked_2 = ui->radioSolver_2->blockSignals(true);
    bool blocked_3 = ui->radioRBS_1->blockSignals(true);
    bool blocked_4 = ui->radioRBS_2->blockSignals(true);

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
    ui->radioRBS_1->setEnabled(ui->radioSolver_1->isChecked());
    ui->radioRBS_2->setEnabled(ui->radioSolver_1->isChecked());

    ui->radioSolver_0->blockSignals(blocked_0);
    ui->radioSolver_1->blockSignals(blocked_1);
    ui->radioSolver_2->blockSignals(blocked_2);
    ui->radioRBS_1->blockSignals(blocked_3);
    ui->radioRBS_2->blockSignals(blocked_4);
}
