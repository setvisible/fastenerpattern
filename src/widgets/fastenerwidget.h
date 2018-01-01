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

#ifndef WIDGETS_FASTENER_WIDGET_H
#define WIDGETS_FASTENER_WIDGET_H

#include <Core/Fastener>
#include <Widgets/AbstractSpliceView>

class QString;

namespace Ui {
class FastenerWidget;
}

class FastenerWidget : public AbstractSpliceView
{
    Q_OBJECT
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_PROPERTY(Fastener fastener READ fastener WRITE setFastener)
public:
    explicit FastenerWidget(QWidget *parent = 0);
    ~FastenerWidget();

    int decimals() const;
    void setDecimals(int digits);

    Fastener fastener() const;
    void setFastener(const Fastener &fastener);

public Q_SLOTS:
    virtual void onFastenerChanged(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void onSelectionFastenerChanged() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onTextChanged(QString);
    void onValueChanged(double);
    void onStateChanged(int);
    void onChanged();

private:
    Ui::FastenerWidget *ui;
    int m_currentIndex;
};

#endif // WIDGETS_FASTENER_WIDGET_H
