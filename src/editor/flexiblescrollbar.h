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

#ifndef EDITOR_FLEXIBLE_SCROLLBAR_H
#define EDITOR_FLEXIBLE_SCROLLBAR_H

/*
 * Inspired by Tiled (Tile Editor) in /src/tiled/flexiblescrollbar.cpp
 */

#include <QScrollBar>

class FlexibleScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit FlexibleScrollBar(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);

    void forceSetValue(int value);
    void allowNextRangeChange();

protected:
    void sliderChange(SliderChange change) override;

private:
    void setOverrideMinimum(int min);
    void setOverrideMaximum(int max);

    int mOverrideMinimum;
    int mOverrideMaximum;
    int mDesiredMinimum;
    int mDesiredMaximum;
    bool mAllowRangeChange;
};

#endif // EDITOR_FLEXIBLE_SCROLLBAR_H
