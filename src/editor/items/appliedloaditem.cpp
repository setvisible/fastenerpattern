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

#include "appliedloaditem.h"

#include "arcarrowitem.h"
#include "arrowitem.h"

#include <QtGui/QBrush>

AppliedLoadItem::AppliedLoadItem(QGraphicsItem *parent) : TensorItem(parent)
{
    setInvertedArrow(true);

    QColor magenta(221,0,166);
    m_arrowXY->setColor( magenta );
    m_arrowX->setColor( magenta );
    m_arrowY->setColor( magenta );
    m_arcArrowZ->setColor( magenta );
    m_labelXY->setBrush(QBrush( magenta ));
    m_labelX->setBrush(QBrush( magenta ));
    m_labelY->setBrush(QBrush( magenta ));
    m_labelZ->setBrush(QBrush( magenta ));
}

