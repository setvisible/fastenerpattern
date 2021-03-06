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

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <Widgets/AppliedLoadWidget>
#include <Widgets/DesignObjectiveWidget>
#include <Widgets/DesignOptionWidget>
#include <Widgets/DesignSpaceWidget>
#include <Widgets/DesignVariableWidget>
#include <Widgets/FastenerWidget>
#include <Widgets/FastenerTableWidget>
#include <Widgets/OptimisationWidget>
#include <Widgets/ResultWidget>
#include <Widgets/SolverWidget>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

AppliedLoadWidget* MainWidget::appliedLoadWidget() const
{
    return ui->appliedLoadWidget;
}

DesignObjectiveWidget* MainWidget::designObjectiveWidget() const
{
    return ui->designObjectiveWidget;
}

DesignOptionWidget* MainWidget::designOptionWidget() const
{
    return ui->designOptionWidget;
}

DesignSpaceWidget* MainWidget::designSpaceWidget() const
{
    return ui->designSpaceWidget;
}

DesignVariableWidget* MainWidget::designVariableWidget() const
{
    return ui->designVariableWidget;
}

OptimisationWidget* MainWidget::optimisationWidget() const
{
    return ui->optimisationWidget;
}

FastenerWidget* MainWidget::fastenerWidget() const
{
    return ui->fastenerWidget;
}

FastenerTableWidget* MainWidget::fastenerTableWidget() const
{
    return ui->fastenerTableWidget;
}

ResultWidget* MainWidget::resultWidget() const
{
    return ui->resultWidget;
}

SolverWidget* MainWidget::solverWidget() const
{
    return ui->solverWidget;
}

int MainWidget::currentIndex() const
{
    return ui->tabWidget->currentIndex();
}

void MainWidget::setCurrentIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}
