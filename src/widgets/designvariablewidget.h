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

#ifndef WIDGETS_DESIGN_VARIABLE_WIDGET_H
#define WIDGETS_DESIGN_VARIABLE_WIDGET_H

#include <Widgets/AbstractSpliceView>

class QTimer;

class Fastener;

namespace Ui {
class DesignVariableWidget;
}

class DesignVariableWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit DesignVariableWidget(QWidget *parent = 0);
    ~DesignVariableWidget();

public Q_SLOTS:
    virtual void onFastenerInserted(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void onFastenerChanged(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void onFastenerRemoved(const int index) Q_DECL_OVERRIDE;
    virtual void onSelectionFastenerChanged() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onItemSelectionChanged();
    void updateTable();
    void updateSelection();

private:
    Ui::DesignVariableWidget *ui;
    QTimer *m_tableTimer;
    QTimer *m_selectionTimer;
    void resizeColumnToContents();
    void updateTableLater(int msec = 100);
    void updateSelectionLater(int msec = 100);

};

#endif // WIDGETS_DESIGN_VARIABLE_WIDGET_H
