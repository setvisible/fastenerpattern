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

#ifndef UTILS_DELAY_TIMER_H
#define UTILS_DELAY_TIMER_H

#include <Core/Fastener>

#include <QtCore/QObject>

class QTimer;

class DelayTimer : public QObject
{
    Q_OBJECT
public:
    explicit DelayTimer(QObject *parent = Q_NULLPTR);

    int updateDelay() const;
    void setUpdateDelay(const int msec);

Q_SIGNALS:
    void onFastenerChanged_timeout(const int index, const Fastener &fastener);
    void onSelectionFastenerChanged_timeout();

public Q_SLOTS:
    void onFastenerChanged_delayed(const int index, const Fastener &fastener);
    void onSelectionFastenerChanged_delayed();

private Q_SLOTS:
    void shot_now_onFastenerChanged();
    void shot_now_onSelectionFastenerChanged();

private:
    int m_delay; // in msec
    QTimer *m_timer_onFastenerChanged;
    QTimer *m_timer_onSelectionFastenerChanged;

    int m_index;
    Fastener m_fastener;
};


#endif // UTILS_DELAY_TIMER_H
