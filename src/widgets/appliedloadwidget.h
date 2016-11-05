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

#ifndef WIDGETS_APPLIED_LOAD_WIDGET_H
#define WIDGETS_APPLIED_LOAD_WIDGET_H

#include <Widgets/AbstractSpliceView>

namespace Ui {
class AppliedLoadWidget;
}

class AppliedLoadWidget : public AbstractSpliceView
{
    Q_OBJECT
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_PROPERTY(Tensor appliedLoad READ appliedLoad WRITE setAppliedLoad)
public:
    explicit AppliedLoadWidget(QWidget *parent = 0);
    ~AppliedLoadWidget();

    int decimals() const;
    void setDecimals(int digits);

    Tensor appliedLoad() const;
    void setAppliedLoad(const Tensor &appliedLoad);

public Q_SLOTS:
    virtual void appliedLoadChanged() Q_DECL_OVERRIDE;

protected Q_SLOTS:
    void onValueChanged(double);

private:
    Ui::AppliedLoadWidget *ui;
};

#endif // WIDGETS_APPLIED_LOAD_WIDGET_H
