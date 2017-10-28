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

#include "handleitem.h"

#include <QtCore/QDebug>
#include <QtGui/QCursor>
#include <QtGui/QPainter>

/*! \class HandleItem
 *  \brief The class HandleItem is a graphics item representing
 *         a movable handle.
 */

HandleItem::HandleItem(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setCursor(Qt::SizeVerCursor);
    this->setZValue(500);
}

QRectF HandleItem::boundingRect() const
{
    return shape().boundingRect();
}
QPainterPath HandleItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(-6, -6, 14, 14)); /* two times bigger than the drawn rect */
    return path;
}
void HandleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(QColor(255,127,39), Qt::SolidPattern)); /* orange */
    painter->drawRect(QRect(-3, -3, 7, 7));
}


