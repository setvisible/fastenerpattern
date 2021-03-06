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

#ifndef WIDGETS_RESULT_WIDGET_H
#define WIDGETS_RESULT_WIDGET_H

#include <Widgets/AbstractSpliceView>

class QTimer;

namespace Ui {
class ResultWidget;
}

class ResultWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();

public Q_SLOTS:
    virtual void onSelectionFastenerChanged() Q_DECL_OVERRIDE;
    virtual void onResultsChanged() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onItemSelectionChanged();
    void updateSelection();
    void updateResult();

private:
    Ui::ResultWidget *ui;
    QTimer *m_selectionTimer;
    QTimer *m_resultTimer;
    void resizeColumnToContents();
    void updateSelectionLater(int msec = 100);
    void updateResultLater(int msec = 100);
};

#endif // WIDGETS_RESULT_WIDGET_H
