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

#include "abstractspliceview.h"

#include <Core/AbstractSpliceModel>
#include <Core/DesignSpace>
#include <Core/Fastener>
#include <Utils/DelayTimer>

/*! \class AbstractSpliceView
 *  \brief The class AbstractSpliceView is a view of the AbstractSpliceModel.
 *
 * \sa AbstractSpliceModel
 */

AbstractSpliceView::AbstractSpliceView(QWidget *parent) : QWidget(parent)
  , m_model(Q_NULLPTR)
  , m_delayTimer(new DelayTimer(this))
{
}

/******************************************************************************
 ******************************************************************************/
int AbstractSpliceView::updateDelay() const
{
    return m_delayTimer->updateDelay();
}

void AbstractSpliceView::setUpdateDelay(const int msec)
{
    m_delayTimer->setUpdateDelay(msec);
}

/******************************************************************************
 ******************************************************************************/
AbstractSpliceModel *AbstractSpliceView::model() const
{
    return m_model;
}

void AbstractSpliceView::setModel(AbstractSpliceModel *model)
{
    if (model == m_model)
        return;

    if (m_model) {
        QObject::disconnect(m_model, SIGNAL(fastenerInserted(int,Fastener)),
                            m_delayTimer, SLOT(onFastenerInserted_delayed(int,Fastener)));
        QObject::disconnect(m_delayTimer, SIGNAL(onFastenerInserted_timeout(int,Fastener)),
                            this, SLOT(onFastenerInserted(int,Fastener)));
        QObject::disconnect(m_model, SIGNAL(fastenerChanged(int,Fastener)),
                            m_delayTimer, SLOT(onFastenerChanged_delayed(int,Fastener)));
        QObject::disconnect(m_delayTimer, SIGNAL(onFastenerChanged_timeout(int,Fastener)),
                            this, SLOT(onFastenerChanged(int,Fastener)));
        QObject::disconnect(m_model, SIGNAL(fastenerRemoved(int)),
                            m_delayTimer, SLOT(onFastenerRemoved_delayed(int)));
        QObject::disconnect(m_delayTimer, SIGNAL(onFastenerRemoved_timeout(int)),
                            this, SLOT(onFastenerRemoved(int)));

        QObject::disconnect(m_model, SIGNAL(designSpaceInserted(int,DesignSpace)),
                            this, SLOT(onDesignSpaceInserted(int,DesignSpace)));
        QObject::disconnect(m_model, SIGNAL(designSpaceChanged(int,DesignSpace)),
                            this, SLOT(onDesignSpaceChanged(int,DesignSpace)));
        QObject::disconnect(m_model, SIGNAL(designSpaceRemoved(int)),
                            this, SLOT(onDesignSpaceRemoved(int)));

        QObject::disconnect(m_model, SIGNAL(appliedLoadChanged()),
                            this, SLOT(onAppliedLoadChanged()));
        QObject::disconnect(m_model, SIGNAL(resultsChanged()),
                            m_delayTimer, SLOT(onResultsChanged_delayed()));
        QObject::disconnect(m_delayTimer, SIGNAL(onResultsChanged_timeout()),
                            this, SLOT(onResultsChanged()));

        QObject::disconnect(m_model, SIGNAL(selectionFastenerChanged()),
                            m_delayTimer, SLOT(onSelectionFastenerChanged_delayed()));
        QObject::disconnect(m_delayTimer, SIGNAL(onSelectionFastenerChanged_timeout()),
                            this, SLOT(onSelectionFastenerChanged()));
        QObject::disconnect(m_model, SIGNAL(selectionDesignSpaceChanged()),
                            this, SLOT(onSelectionDesignSpaceChanged()));
    }
    m_model = model;
    if (m_model) {
        QObject::connect(m_model, SIGNAL(fastenerInserted(int,Fastener)),
                         m_delayTimer, SLOT(onFastenerInserted_delayed(int,Fastener)));
        QObject::connect(m_delayTimer, SIGNAL(onFastenerInserted_timeout(int,Fastener)),
                         this, SLOT(onFastenerInserted(int,Fastener)));
        QObject::connect(m_model, SIGNAL(fastenerChanged(int,Fastener)),
                         m_delayTimer, SLOT(onFastenerChanged_delayed(int,Fastener)));
        QObject::connect(m_delayTimer, SIGNAL(onFastenerChanged_timeout(int,Fastener)),
                         this, SLOT(onFastenerChanged(int,Fastener)));
        QObject::connect(m_model, SIGNAL(fastenerRemoved(int)),
                         m_delayTimer, SLOT(onFastenerRemoved_delayed(int)));
        QObject::connect(m_delayTimer, SIGNAL(onFastenerRemoved_timeout(int)),
                         this, SLOT(onFastenerRemoved(int)));

        QObject::connect(m_model, SIGNAL(designSpaceInserted(int,DesignSpace)),
                         this, SLOT(onDesignSpaceInserted(int,DesignSpace)));
        QObject::connect(m_model, SIGNAL(designSpaceChanged(int,DesignSpace)),
                         this, SLOT(onDesignSpaceChanged(int,DesignSpace)));
        QObject::connect(m_model, SIGNAL(designSpaceRemoved(int)),
                         this, SLOT(onDesignSpaceRemoved(int)));

        QObject::connect(m_model, SIGNAL(appliedLoadChanged()),
                         this, SLOT(onAppliedLoadChanged()));
        QObject::connect(m_model, SIGNAL(resultsChanged()),
                         m_delayTimer, SLOT(onResultsChanged_delayed()));
        QObject::connect(m_delayTimer, SIGNAL(onResultsChanged_timeout()),
                         this, SLOT(onResultsChanged()));

        QObject::connect(m_model, SIGNAL(selectionFastenerChanged()),
                         m_delayTimer, SLOT(onSelectionFastenerChanged_delayed()));
        QObject::connect(m_delayTimer, SIGNAL(onSelectionFastenerChanged_timeout()),
                         this, SLOT(onSelectionFastenerChanged()));
        QObject::connect(m_model, SIGNAL(selectionDesignSpaceChanged()),
                         this, SLOT(onSelectionDesignSpaceChanged()));



    }
}

/******************************************************************************
 ******************************************************************************/
void AbstractSpliceView::onFastenerInserted(const int, const Fastener &)
{
}

void AbstractSpliceView::onFastenerChanged(const int, const Fastener &)
{
}

void AbstractSpliceView::onFastenerRemoved(const int)
{
}

/******************************************************************************
 ******************************************************************************/
void AbstractSpliceView::onDesignSpaceInserted(const int, const DesignSpace &)
{
}

void AbstractSpliceView::onDesignSpaceChanged(const int, const DesignSpace &)
{
}

void AbstractSpliceView::onDesignSpaceRemoved(const int)
{
}

/******************************************************************************
 ******************************************************************************/
void AbstractSpliceView::onAppliedLoadChanged()
{
}

void AbstractSpliceView::onResultsChanged()
{
}

/******************************************************************************
 ******************************************************************************/
void AbstractSpliceView::onSelectionFastenerChanged()
{
}

void AbstractSpliceView::onSelectionDesignSpaceChanged()
{
}
