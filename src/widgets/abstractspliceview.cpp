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

/*! \class AbstractSpliceView
 *  \brief The class AbstractSpliceView is a view of the AbstractSpliceModel.
 *
 * \sa AbstractSpliceModel
 */

AbstractSpliceView::AbstractSpliceView(QWidget *parent) : QWidget(parent)
  , m_model(Q_NULLPTR)
{
}

AbstractSpliceModel *AbstractSpliceView::model() const
{
    return m_model;
}

void AbstractSpliceView::setModel(AbstractSpliceModel *model)
{
    if (model == m_model)
        return;

    if (m_model) {
        QObject::disconnect(m_model, SIGNAL(appliedLoadChanged()),
                            this, SLOT(appliedLoadChanged()));

        QObject::disconnect(m_model, SIGNAL(fastenersChanged(int,Fastener)),
                            this, SLOT(fastenersChanged(int,Fastener)));

        QObject::disconnect(m_model, SIGNAL(fastenersInserted(int,Fastener)),
                            this, SLOT(fastenersInserted(int,Fastener)));

        QObject::disconnect(m_model, SIGNAL(fastenersRemoved(int)),
                            this, SLOT(fastenersRemoved(int)));

        QObject::disconnect(m_model, SIGNAL(selectionFastenerChanged()),
                            this, SLOT(selectionFastenerChanged()));

        QObject::disconnect(m_model, SIGNAL(resultsChanged()),
                            this, SLOT(resultsChanged()));
    }
    m_model = model;
    if (m_model) {
        QObject::connect(m_model, SIGNAL(appliedLoadChanged()),
                         this, SLOT(appliedLoadChanged()));

        QObject::connect(m_model, SIGNAL(fastenersChanged(int,Fastener)),
                         this, SLOT(fastenersChanged(int,Fastener)));

        QObject::connect(m_model, SIGNAL(fastenersInserted(int,Fastener)),
                         this, SLOT(fastenersInserted(int,Fastener)));

        QObject::connect(m_model, SIGNAL(fastenersRemoved(int)),
                         this, SLOT(fastenersRemoved(int)));

        QObject::connect(m_model, SIGNAL(selectionFastenerChanged()),
                         this, SLOT(selectionFastenerChanged()));

        QObject::connect(m_model, SIGNAL(resultsChanged()),
                         this, SLOT(resultsChanged()));

    }
}

void AbstractSpliceView::appliedLoadChanged()
{
}

void AbstractSpliceView::fastenersInserted(const int /*index*/, const Fastener &/*fastener*/)
{
}

void AbstractSpliceView::fastenersChanged(const int /*index*/, const Fastener &/*fastener*/)
{
}

void AbstractSpliceView::fastenersRemoved(const int /*index*/)
{
}

void AbstractSpliceView::selectionFastenerChanged()
{
}

void AbstractSpliceView::resultsChanged()
{
}
