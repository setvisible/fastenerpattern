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

#include "tensoritem.h"

#include "arcarrowitem.h"
#include "arrowitem.h"
#include <Core/Tensor>

#include <QtCore/QDebug>
#include <QtGui/QPainter>

TensorItem::TensorItem(QGraphicsItem *parent) : QGraphicsItemGroup(parent)
  , m_arrowXY(new ArrowItem(this))
  , m_arrowX(new ArrowItem(this))
  , m_arrowY(new ArrowItem(this))
  , m_arcArrowZ(new ArcArrowItem(this))
  , m_isArrowInverted(false)
  , m_isComponantVisible(true)
  , m_isResultantVisible(true)
  , m_isTorqueVisible(true)
  , m_loadScaleFactor(1.0)
  , m_torqueScaleFactor(1.0)
  , m_fx(0.0)
  , m_fy(0.0)
  , m_tz(0.0)
{
   this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->setZValue(100);

    m_arrowXY->setColor(QColor(255,127,39));    // orange
    m_arrowX->setColor(QColor(237,28,36));      // red
    m_arrowY->setColor(QColor(34,177,76));      // green
    m_arcArrowZ->setColor(QColor(0,162,232));   // blue
}

bool TensorItem::isArrowInverted() const
{
    return m_isArrowInverted;
}

void TensorItem::setInvertedArrow(bool inverted)
{
    m_isArrowInverted = inverted;
    updateArrows();
}

bool TensorItem::isComponantVisible() const
{
    return m_isComponantVisible;
}

void TensorItem::setComponantVisible(bool visible)
{
    m_isComponantVisible = visible;
    updateArrows();
}

bool TensorItem::isResultantVisible() const
{
    return m_isResultantVisible;
}

void TensorItem::setResultantVisible(bool visible)
{
    m_isResultantVisible = visible;
    updateArrows();
}

bool TensorItem::isTorqueVisible() const
{
    return m_isTorqueVisible;
}
void TensorItem::setTorqueVisible(bool visible)
{
    m_isTorqueVisible = visible;
    updateArrows();
}

qreal TensorItem::loadScaleFactor() const
{
    return m_loadScaleFactor;
}

void TensorItem::setLoadScaleFactor(qreal factor)
{
    m_loadScaleFactor = factor;
    updateArrows();
}

qreal TensorItem::torqueScaleFactor() const
{
    return m_torqueScaleFactor;
}

void TensorItem::setTorqueScaleFactor(qreal factor)
{
    m_torqueScaleFactor = factor;
    updateArrows();
}

Tensor TensorItem::tensor() const
{
    Tensor tensor(m_fx *N, m_fy *N, m_tz *N_m);
    return tensor;
}

void TensorItem::setTensor(const Tensor &tensor)
{
    m_fx = tensor.force_x.value();
    m_fy = tensor.force_y.value();
    m_tz = tensor.torque_z.value();
    updateArrows();
}

void TensorItem::updateArrows()
{
    Q_ASSERT(m_arrowXY);
    Q_ASSERT(m_arrowX);
    Q_ASSERT(m_arrowY);
    Q_ASSERT(m_arcArrowZ);

    m_arrowXY->setVisible(m_isResultantVisible);
    m_arrowX->setVisible(m_isComponantVisible);
    m_arrowY->setVisible(m_isComponantVisible);
    m_arcArrowZ->setVisible(m_isTorqueVisible);

    /// \todo Use pow(1.000152, factor) instead ?

    /// \todo  m_fx, m_fy... can be 0, NaN of Infinite. What to do in such case ?

    if (m_isArrowInverted) {
        m_arrowXY->setLine(QLineF(m_fx*m_loadScaleFactor, -1* m_fy*m_loadScaleFactor, 0, 0));
        m_arrowX->setLine(QLineF( m_fx*m_loadScaleFactor, 0, 0, 0));
        m_arrowY->setLine(QLineF(0, -1*m_fy*m_loadScaleFactor, 0, 0));
    } else {
        m_arrowXY->setLine(QLineF(0, 0, m_fx*m_loadScaleFactor, -1*m_fy*m_loadScaleFactor));
        m_arrowX->setLine(QLineF(0, 0, m_fx*m_loadScaleFactor, 0));
        m_arrowY->setLine(QLineF(0, 0, 0, -1*m_fy*m_loadScaleFactor));
    }

    m_arcArrowZ->setRect(-25,-25, 50, 50);
    m_arcArrowZ->setStartAngle(-10 * 16);
    m_arcArrowZ->setSpanAngle( 20 * 16);    /* test */

    QGraphicsItem::update();
}

