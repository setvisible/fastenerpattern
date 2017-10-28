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

#include "fasteneritem.h"
#include "tensoritem.h"
#include "symbolitem.h"
#include "utils_scale.h"

#include <QtCore/QDebug>
#include <QtGui/QCursor>
#include <QtGui/QPainter>

FastenerItem::FastenerItem(QGraphicsItem *parent) : QGraphicsObject(parent)
  , m_tensorItem(new TensorItem(this))
  , m_symbolItem(new SymbolItem(this))
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->setCursor(Qt::SizeAllCursor);

    m_symbolItem->setZValue(100);
}

QRectF FastenerItem::boundingRect() const
{
    Q_ASSERT(m_symbolItem);
    return m_symbolItem->boundingRect();
}

QPainterPath FastenerItem::shape() const
{
    Q_ASSERT(m_symbolItem);
    return m_symbolItem->shape();
}

void FastenerItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

qreal FastenerItem::truePositionX() const
{
    return (this->x() / C_DEFAULT_SCREEN_DPI);
}

qreal FastenerItem::truePositionY() const
{
    return (-1 * this->y() / C_DEFAULT_SCREEN_DPI);
}

void FastenerItem::setTruePosition(qreal x, qreal y)
{
    QGraphicsItem::setPos( x * C_DEFAULT_SCREEN_DPI,
                           -1 * y * C_DEFAULT_SCREEN_DPI);
    QGraphicsItem::update();
}

qreal FastenerItem::trueDiameter() const
{
    Q_ASSERT(m_symbolItem);
    return (m_symbolItem->diameter() / C_DEFAULT_SCREEN_DPI);
}

void FastenerItem::setTrueDiameter(qreal diameter)
{
    Q_ASSERT(m_symbolItem);
    m_symbolItem->setDiameter(diameter * C_DEFAULT_SCREEN_DPI);
    QGraphicsItem::update();
}

Tensor FastenerItem::result() const
{
    Q_ASSERT(m_tensorItem);
    return m_tensorItem->tensor();
}

void FastenerItem::setResult(const Tensor &tensor)
{    
    Q_ASSERT(m_tensorItem);
    m_tensorItem->setTensor(tensor);
    QGraphicsItem::update();
}

bool FastenerItem::isComponentVisible() const
{
    Q_ASSERT(m_tensorItem);
    return m_tensorItem->isComponentVisible();
}

void FastenerItem::setComponentVisible(bool visible)
{
    Q_ASSERT(m_tensorItem);
    m_tensorItem->setComponentVisible(visible);
}

bool FastenerItem::isResultantVisible() const
{
    Q_ASSERT(m_tensorItem);
    return m_tensorItem->isResultantVisible();
}

void FastenerItem::setResultantVisible(bool visible)
{
    Q_ASSERT(m_tensorItem);
    m_tensorItem->setResultantVisible(visible);
}

bool FastenerItem::isTorqueVisible() const
{
    Q_ASSERT(m_tensorItem);
    return m_tensorItem->isTorqueVisible();
}

void FastenerItem::setTorqueVisible(bool visible)
{
    Q_ASSERT(m_tensorItem);
    m_tensorItem->setTorqueVisible(visible);
}

bool FastenerItem::isLabelVisible() const
{
    Q_ASSERT(m_tensorItem);
    return m_tensorItem->isLabelVisible();
}

void FastenerItem::setLabelVisible(bool visible)
{
    Q_ASSERT(m_tensorItem);
    m_tensorItem->setLabelVisible(visible);
}
