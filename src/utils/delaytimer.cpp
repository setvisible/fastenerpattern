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

#include "delaytimer.h"

#include <QtCore/QTimer>

/*! \class DelayTimer
 * \brief The class DelayTimer installs a delay between some signals and slots.
 *
 * The short delay created by the DelayTimer avoids the issue with some
 * GUI widgets deriving from the class \a AbstractSpliceView that refresh
 * too often and too early.
 *
 * This increases the GUI response.
 */

DelayTimer::DelayTimer(QObject *parent)
    : QObject(parent)
    , m_delay(100)
    , m_timer_onFastenerInserted(new QTimer(this))
    , m_timer_onFastenerChanged(new QTimer(this))
    , m_timer_onFastenerRemoved(new QTimer(this))
    , m_timer_onSelectionFastenerChanged(new QTimer(this))
    , m_timer_onResultsChanged(new QTimer(this))
{
    connect(m_timer_onFastenerInserted, SIGNAL(timeout()),
            this, SLOT(shot_now_onFastenerInserted()));
    connect(m_timer_onFastenerChanged, SIGNAL(timeout()),
            this, SLOT(shot_now_onFastenerChanged()));
    connect(m_timer_onFastenerRemoved, SIGNAL(timeout()),
            this, SLOT(shot_now_onFastenerRemoved()));

    connect(m_timer_onSelectionFastenerChanged, SIGNAL(timeout()),
            this, SLOT(shot_now_onSelectionFastenerChanged()));

    connect(m_timer_onResultsChanged, SIGNAL(timeout()),
            this, SLOT(shot_now_onResultsChanged()));
}

/******************************************************************************
 ******************************************************************************/
int DelayTimer::updateDelay() const
{
    return m_delay;
}

void DelayTimer::setUpdateDelay(const int msec)
{
    m_delay = msec;
}

/******************************************************************************
 ******************************************************************************/
void DelayTimer::onFastenerInserted_delayed(const int index, const Fastener &fastener)
{
    m_indexInserted = index;
    m_fastenerInserted = fastener;
    m_timer_onFastenerInserted->stop();
    m_timer_onFastenerInserted->start(m_delay);
}

void DelayTimer::shot_now_onFastenerInserted()
{
    m_timer_onFastenerInserted->stop();
    emit onFastenerInserted_timeout(m_indexInserted, m_fastenerInserted);
}

/******************************************************************************
 ******************************************************************************/
void DelayTimer::onFastenerChanged_delayed(const int index, const Fastener &fastener)
{
    m_indexChanged = index;
    m_fastenerChanged = fastener;
    m_timer_onFastenerChanged->stop();
    m_timer_onFastenerChanged->start(m_delay);
}

void DelayTimer::shot_now_onFastenerChanged()
{
    m_timer_onFastenerChanged->stop();
    emit onFastenerChanged_timeout(m_indexChanged, m_fastenerChanged);
}

/******************************************************************************
 ******************************************************************************/
void DelayTimer::onFastenerRemoved_delayed(const int index)
{
    m_indexRemoved = index;
    m_timer_onFastenerRemoved->stop();
    m_timer_onFastenerRemoved->start(m_delay);
}

void DelayTimer::shot_now_onFastenerRemoved()
{
    m_timer_onFastenerRemoved->stop();
    emit onFastenerRemoved_timeout(m_indexRemoved);
}

/******************************************************************************
 ******************************************************************************/
void DelayTimer::onSelectionFastenerChanged_delayed()
{
    m_timer_onSelectionFastenerChanged->stop();
    m_timer_onSelectionFastenerChanged->start(m_delay);
}

void DelayTimer::shot_now_onSelectionFastenerChanged()
{
    m_timer_onSelectionFastenerChanged->stop();
    emit onSelectionFastenerChanged_timeout();
}

/******************************************************************************
 ******************************************************************************/
void DelayTimer::onResultsChanged_delayed()
{
    m_timer_onResultsChanged->stop();
    m_timer_onResultsChanged->start(m_delay);
}

void DelayTimer::shot_now_onResultsChanged()
{
    m_timer_onResultsChanged->stop();
    emit onResultsChanged_timeout();
}
