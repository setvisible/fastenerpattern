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

#ifndef WIDGETS_SOLVER_WIDGET_H
#define WIDGETS_SOLVER_WIDGET_H

#include <Widgets/AbstractSpliceView>

enum class SolverParameters;

namespace Ui {
class SolverWidget;
}

class SolverWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit SolverWidget(QWidget *parent = 0);
    ~SolverWidget();

public Q_SLOTS:
    virtual void onSolverParamsChanged() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onRadioToggled();

private:
    Ui::SolverWidget *ui;
    SolverParameters fromGUI() const;
    void toGUI(SolverParameters params);
};

#endif // WIDGETS_SOLVER_WIDGET_H
