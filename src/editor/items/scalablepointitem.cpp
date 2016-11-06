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

#include "scalablepointitem.h"

#include <QtCore/QDebug>
#include <QtGui/QPainter>

/// \todo #include <QtGui/QCursor>
/// \todo derive that from #include <QGraphicsRectItem> ?

ScalablePointItem::ScalablePointItem(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    /// \todo this->setRect(-3, -3, 7, 7);
    /// \todo this->setPen(QPen(Qt::black, 1));
    /// \todo this->setBrush(QBrush(QColor(255,127,39), Qt::SolidPattern)); // orange

    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF ScalablePointItem::boundingRect() const
{
    return shape().boundingRect();
}
QPainterPath ScalablePointItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(-3, -3, 7, 7));
    return path;
}
void ScalablePointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(QColor(255,127,39), Qt::SolidPattern)); // orange
    painter->drawRect(QRect(-3, -3, 7, 7));
}


