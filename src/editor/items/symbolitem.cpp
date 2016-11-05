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

#include "symbolitem.h"
#include "utils_scale.h"

#include <QtGui/QPainter>
#include <QtCore/QtMath>    /* qSqrt() */


/*! \class SymbolItem
 *  \brief Draw a symbol of fastener.
 *
 * The SymbolItem is a basically an ellipse centered around its origin (0, 0).
 */
SymbolItem::SymbolItem(QGraphicsItem *parent) : QGraphicsItem(parent)
  , m_diameter(6.35 * C_DEFAULT_SCREEN_DPI)
{
}

qreal SymbolItem::diameter() const
{
    return m_diameter;
}

void SymbolItem::setDiameter(qreal diameter)
{
    m_diameter = diameter;
    QGraphicsItem::update();
}

QRectF SymbolItem::boundingRect() const
{
    const qreal axes = (m_diameter * C_SYMBOL_PERCENT_AXES_LENGTH);
    const qreal penWidth = m_diameter * C_SYMBOL_PERCENT_PEN_WIDTH;
    return QRectF( -axes/2 - penWidth, -axes/2 - penWidth,
                   axes + penWidth, axes + penWidth);

}

/*! \brief Returns the shape of this item as a QPainterPath in local coordinates.
 *
 * \remark We reimplement shape() as an ellipse shape, otherwise the item's hit area
 * would be identical to its bounding rectangle (i.e., rectangular).
 */
QPainterPath SymbolItem::shape() const
{
    QPainterPath path;
    path.addEllipse(-m_diameter/2, -m_diameter/2, m_diameter, m_diameter);
    return path;
}

void SymbolItem::paint(QPainter *painter, const QStyleOptionGraphicsItem  */*option*/, QWidget *)
{
    /// \todo use the Level of Details aka option->levelOfDetail() ?

    bool _isSelected = this->parentItem()->isSelected(); // horrible hack

    QColor green(133,189,60);
    QPen heavyPen( (_isSelected ? green : Qt::black), 2 * m_diameter * C_SYMBOL_PERCENT_PEN_WIDTH);
    QPen lightPen( (_isSelected ? green : Qt::black), 1 * m_diameter * C_SYMBOL_PERCENT_PEN_WIDTH);
    QPen  dashPen( (_isSelected ? green : Qt::black), 2 * m_diameter * C_SYMBOL_PERCENT_PEN_WIDTH,
                   Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);

    painter->setPen(heavyPen);
    painter->setBrush(Qt::NoBrush);

    /* Hexagon */
    const qreal rs       = m_diameter * qSqrt(3.0) / 4.0;
    const qreal half     = m_diameter / 2.0;
    const qreal quarter  = m_diameter / 4.0;

    const QPointF points[6] = {
        QPointF(   0,  half),
        QPointF(  rs,  quarter),
        QPointF(  rs, -quarter),
        QPointF(   0, -half),
        QPointF( -rs, -quarter),
        QPointF( -rs,  quarter)
    };
    painter->drawPolygon(points, 6);

    /* Filet Circle */
    painter->drawEllipse( QRectF(-rs, -rs, 2*rs, 2*rs) );

    /* Hole */
    const qreal hole_size = m_diameter * C_SYMBOL_PERCENT_HOLE_DIAMETER;
    painter->drawEllipse( QRectF(-hole_size/2, -hole_size/2, hole_size, hole_size) );

    /* Hole Thread */
    const qreal thread_size = m_diameter * C_SYMBOL_PERCENT_THREAD_DIAMETER ;
    painter->setPen( lightPen );
    painter->drawArc(QRectF(-thread_size/2, -thread_size/2, thread_size, thread_size),
                     180 * 16, 270 * 16 );

    /* Axes */
    const qreal halfAxes = m_diameter * C_SYMBOL_PERCENT_AXES_LENGTH / 2.0;
    painter->setPen( dashPen );
    painter->drawLine(0, -halfAxes, 0, halfAxes);
    painter->drawLine(-halfAxes, 0, halfAxes, 0);   
}

