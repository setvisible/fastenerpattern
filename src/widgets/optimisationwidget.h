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

#ifndef WIDGETS_OPTIMISATION_WIDGET_H
#define WIDGETS_OPTIMISATION_WIDGET_H

#include <Widgets/AbstractSpliceView>

namespace Ui {
class OptimisationWidget;
}

class OptimisationWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit OptimisationWidget(QWidget *parent = 0);
    virtual ~OptimisationWidget();

public Q_SLOTS:
    virtual void fastenersInserted(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void fastenersChanged(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void fastenersRemoved(const int index) Q_DECL_OVERRIDE;
    virtual void selectionChanged() Q_DECL_OVERRIDE;
    virtual void resultsChanged() Q_DECL_OVERRIDE;

protected Q_SLOTS:
    void onItemSelectionChanged();

private:
    Ui::OptimisationWidget *ui;
};

#endif // WIDGETS_OPTIMISATION_WIDGET_H
