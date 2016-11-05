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

#ifndef WIDGETS_SOLVER_WIDGET_H
#define WIDGETS_SOLVER_WIDGET_H

#include <Core/Solvers/Parameters>
#include <QtWidgets/QWidget>

namespace Ui {
class SolverWidget;
}

class SolverWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SolverParameters params READ params WRITE setParams NOTIFY paramsChanged)
public:
    explicit SolverWidget(QWidget *parent = 0);
    ~SolverWidget();

Q_SIGNALS:
    void paramsChanged(SolverParameters params);

public Q_SLOTS:
    SolverParameters params() const;
    void setParams(SolverParameters params);

protected Q_SLOTS:
    void onRadioToggled();

private:
    Ui::SolverWidget *ui;
    SolverParameters m_params;

    void toGUI(SolverParameters params);
    SolverParameters fromGUI() const;
};

#endif // WIDGETS_SOLVER_WIDGET_H
