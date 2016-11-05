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

#include "arcarrowitem.h"

ArcArrowItem::ArcArrowItem(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
  , m_color(Qt::black)
  , m_diameter(25.)
{
}

QColor ArcArrowItem::color() const
{
    return m_color;
}

void ArcArrowItem::setColor(const QColor &color)
{
    m_color = color;
    QGraphicsItem::update();
}

qreal ArcArrowItem::diameter() const
{
    return m_diameter;
}

void ArcArrowItem::setDiameter(qreal diameter)
{
    m_diameter = diameter;
    QGraphicsItem::update();
}

QRectF ArcArrowItem::boundingRect() const
{
    return this->shape().boundingRect();
}

QPainterPath ArcArrowItem::shape() const
{
    QPainterPath path;
    path.addEllipse(-m_diameter/2, -m_diameter/2, m_diameter, m_diameter);
    return path;
}

void ArcArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_UNUSED(painter);
    /// \todo code it !
}

