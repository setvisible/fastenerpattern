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

#include "measureitem.h"
#include "utils_scale.h"

#include <QtCore/QDebug>
#include <QtCore/qnumeric.h>  /* qIsFinite() */
#include <QtCore/QtMath>      /* M_PI */
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSimpleTextItem>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

/*!
 * \class MeasureItem
 * \brief The class MeasureItem is a graphics item with
 * a double arrow that measure a distance between two points.
 *
 * Example: (A and B are points)
 * \code
 *   .A                .B
 *   |<-----352.2----->|
 * \endcode
 *
 * Use \a setLine() to set the position of the two end points.
 * To get the position of the point A (or B), use \a line().p1()
 * (or \a line().p2()). Use \a setText() and \a text() to set/get the text.
 *
 * \sa ArrowItem, ArcArrowItem
 */
MeasureItem::MeasureItem(QGraphicsItem *parent) : QGraphicsLineItem(parent)
  , m_endSpace(0)
  , m_color(Qt::black)
  , m_label(new QGraphicsSimpleTextItem(this))
{
}

/******************************************************************************
 ******************************************************************************/
QColor MeasureItem::color() const
{
    return m_color;
}

void MeasureItem::setColor(const QColor &color)
{
    m_color = color;
    m_label->setBrush( QBrush(m_color) );
    QGraphicsItem::update();
}

/******************************************************************************
 ******************************************************************************/
int MeasureItem::endSpace() const
{
    return m_endSpace;
}

void MeasureItem::setEndSpace(int space)
{
    m_endSpace = space;
    QGraphicsItem::update();
}

/******************************************************************************
 ******************************************************************************/
QString MeasureItem::text() const
{
    return m_label->text();
}

void MeasureItem::setText(const QString &text)
{
    m_label->setText(text);
    QGraphicsItem::update();
}

/******************************************************************************
 ******************************************************************************/
QRectF MeasureItem::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath MeasureItem::shape() const
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

void MeasureItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (qIsFinite(line().length()) && line().length() > 0.0) {

        /*****************************************************\
        *              --> unitVector()                       *
        *     .A   |<----------- text ---------->|    .B       *
        *     <--->             <---->           <--->        *
        *   endspace             width          endspace      *
        \*****************************************************/

        /* Draw the line */
        painter->setPen(QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(QBrush(m_color, Qt::SolidPattern));


        QLineF modline = this->line();
        QLineF space = modline.unitVector();

        if (modline.length() > 2 * m_endSpace ) {
            space.setLength( m_endSpace );
            QPointF offset = space.p2() - space.p1();
            modline.setP1(modline.p1() + offset);
            modline.setP2(modline.p2() - offset);
        }

        if (modline.length() > 2 * m_endSpace && !m_label->text().isEmpty()) {

            qreal width = m_label->boundingRect().width();
            qreal height = m_label->boundingRect().height();

            QLineF line1 = modline;
            QLineF line2 = modline;
            QPointF center = 0.5 * ( modline.p1() + modline.p2() );


            space.setLength( 0.5 * qMax(height, width) + 2.0 );
            QPointF offset = space.p2() - space.p1();

            line1.setP2(center - offset);
            line2.setP1(center + offset);

            painter->drawLine(line1);
            painter->drawLine(line2);

        } else {
            painter->drawLine(modline);
        }

        /* Draw the arrows */
        const qreal angle = this->line().angle() / 180 * M_PI;
        {
            const QPointF p0 = modline.p1();
            const QPointF p1 = p0 + QPointF(
                        std::cos(-angle + (M_PI * 0.1)) * C_ARROW_SIZE,
                        std::sin(-angle + (M_PI * 0.1)) * C_ARROW_SIZE);
            const QPointF p2 = p0 + QPointF(
                        std::cos(-angle - (M_PI * 0.1)) * C_ARROW_SIZE,
                        std::sin(-angle - (M_PI * 0.1)) * C_ARROW_SIZE);

            painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
        }
        {
            const QPointF p0 = modline.p2();
            const QPointF p1 = p0 - QPointF(
                        std::cos(-angle + (M_PI * 0.1)) * C_ARROW_SIZE,
                        std::sin(-angle + (M_PI * 0.1)) * C_ARROW_SIZE);
            const QPointF p2 = p0 - QPointF(
                        std::cos(-angle - (M_PI * 0.1)) * C_ARROW_SIZE,
                        std::sin(-angle - (M_PI * 0.1)) * C_ARROW_SIZE);

            painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
        }

        /* Draw the text */
        if (m_label->text().isEmpty()) {
            m_label->setVisible(false);
        } else {
            m_label->setVisible(true);
            QPointF center = this->boundingRect().center();
            QPointF offset = m_label->boundingRect().center();
            m_label->setPos(center - offset);
        }
    }
}

