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
#include "utils_scale.h"

#include <QtCore/QDebug>
#include <QtGui/QCursor>
#include <QtGui/QPainter>

#include <QtWidgets/QGraphicsSimpleTextItem>

/*! \class HandleItem
 *  \brief The class HandleItem is a graphics item representing a movable handle.
 *
 * Optionally, use setCoordinateVisible() to show the coordinates
 * of the HandleItem.
 *
 */

HandleItem::HandleItem(QGraphicsItem *parent) : QGraphicsObject(parent)
  , m_labelItem(new QGraphicsSimpleTextItem(this))
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setCursor(Qt::SizeVerCursor);
    this->setZValue(500);

    //m_labelItem->setText(QLatin1String("Dummy Text..."));
    const QFontMetrics fm( m_labelItem->font() );
    const int pixelsHigh = fm.height();
    m_labelItem->setPos(10, -0.5 * pixelsHigh - 1);
    m_labelItem->setVisible(false);
    connect(this->parentObject(), SIGNAL(xChanged()), this, SLOT(onPosChanged()));
    connect(this->parentObject(), SIGNAL(yChanged()), this, SLOT(onPosChanged()));
    connect(this, SIGNAL(xChanged()), this, SLOT(onPosChanged()));
    connect(this, SIGNAL(yChanged()), this, SLOT(onPosChanged()));

    onPosChanged();
}

/******************************************************************************
 ******************************************************************************/
bool HandleItem::isCoordinateVisible() const
{
    return m_labelItem->isVisible();
}

void HandleItem::setCoordinateVisible(bool visible)
{
    m_labelItem->setVisible(visible);
}

/******************************************************************************
 ******************************************************************************/
void HandleItem::onPosChanged()
{
    QString text =
            QString("(%0,%1)")
            .arg(this->scenePos().x() / C_DEFAULT_SCREEN_DPI, 0, 'f', 1)
            .arg(-1 * this->scenePos().y() / C_DEFAULT_SCREEN_DPI, 0, 'f', 1);
    m_labelItem->setText(text);
}

/******************************************************************************
 ******************************************************************************/
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


