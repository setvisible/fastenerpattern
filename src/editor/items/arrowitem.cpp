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

#include "arrowitem.h"
#include "utils_scale.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QtCore/QtMath>      /* M_PI */
#include <QtCore/qnumeric.h>  /* qIsFinite() */
#include <QDebug>

ArrowItem::ArrowItem(QGraphicsItem *parent) : QGraphicsLineItem(parent)
  , m_color(Qt::black)
{
}

QColor ArrowItem::color() const
{
    return m_color;
}

void ArrowItem::setColor(const QColor &color)
{
    m_color = color;
    QGraphicsItem::update();
}

QRectF ArrowItem::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath ArrowItem::shape() const
{
    QPainterPath path;
    if (line().length() > 0.0) {
        QPainterPathStroker stroker;
        path.moveTo(line().p1());
        path.lineTo(line().p2());
        stroker.setWidth(C_ARROW_SIZE);
        return stroker.createStroke(path);
    }
    return path;
}

void ArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (qIsFinite(line().length()) && line().length() > 0.0) {

        painter->setPen(QPen(m_color, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->setBrush(QBrush(m_color, Qt::SolidPattern));

        painter->drawLine(this->line());

        double angle = line().angle() / 180 * M_PI;

        const QPointF p0 = line().p2();
        const QPointF p1 = p0 - QPointF(
                    std::cos(-angle + (M_PI * 0.1)) * C_ARROW_SIZE,
                    std::sin(-angle + (M_PI * 0.1)) * C_ARROW_SIZE);
        const QPointF p2 = p0 - QPointF(
                    std::cos(-angle - (M_PI * 0.1)) * C_ARROW_SIZE,
                    std::sin(-angle - (M_PI * 0.1)) * C_ARROW_SIZE);

        painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
    }
}

