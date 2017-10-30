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

#ifndef WIDGETS_DESIGN_SPACE_WIDGET_H
#define WIDGETS_DESIGN_SPACE_WIDGET_H

#include <Widgets/AbstractSpliceView>

class QTableWidgetItem;

namespace Ui {
class DesignSpaceWidget;
}
class DesignSpaceWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit DesignSpaceWidget(QWidget *parent = 0);
    ~DesignSpaceWidget();

public Q_SLOTS:
    virtual void onDesignSpaceInserted(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void onDesignSpaceChanged(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void onDesignSpaceRemoved(const int index) Q_DECL_OVERRIDE;
    virtual void onSelectionDesignSpaceChanged() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onItemDataChanged(QTableWidgetItem *item);
    void onItemSelectionChanged();

private:
    Ui::DesignSpaceWidget *ui;

};

#endif // WIDGETS_DESIGN_SPACE_WIDGET_H
