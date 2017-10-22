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

#ifndef WIDGETS_MAIN_WIDGET_H
#define WIDGETS_MAIN_WIDGET_H

#include <QtWidgets/QWidget>

class AppliedLoadWidget;
class DesignObjectiveWidget;
class DesignOptionWidget;
class DesignSpaceWidget;
class DesignVariableWidget;
class FastenerWidget;
class OptimisationWidget;
class ResultWidget;
class SolverWidget;
class TableWidget;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    AppliedLoadWidget* appliedLoadWidget() const;
    DesignObjectiveWidget* designObjectiveWidget() const;
    DesignOptionWidget* designOptionWidget() const;
    DesignSpaceWidget* designSpaceWidget() const;
    DesignVariableWidget* designVariableWidget() const;
    FastenerWidget* fastenerWidget() const;
    OptimisationWidget* optimisationWidget() const;
    ResultWidget* resultWidget() const;
    SolverWidget* solverWidget() const;
    TableWidget* tableWidget() const;

private:
    Ui::MainWidget *ui;
};

#endif // WIDGETS_MAIN_WIDGET_H
