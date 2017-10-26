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
#include "utils_scale.h"
#include <Core/Tensor>

#include <QtCore/QDebug>
#include <QtCore/QtMath>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSimpleTextItem>


TensorItem::TensorItem(QGraphicsItem *parent) : QGraphicsItemGroup(parent)
  , m_arrowXY(new ArrowItem(this))
  , m_arrowX(new ArrowItem(this))
  , m_arrowY(new ArrowItem(this))
  , m_arcArrowZ(new ArcArrowItem(this))
  , m_labelXY(new QGraphicsSimpleTextItem(this))
  , m_labelX(new QGraphicsSimpleTextItem(this))
  , m_labelY(new QGraphicsSimpleTextItem(this))
  , m_labelZ(new QGraphicsSimpleTextItem(this))
  , m_arrowInverted(false)
  , m_componantVisible(false)
  , m_resultantVisible(true)
  , m_torqueVisible(true)
  , m_labelVisible(false)
  , m_loadScaleFactor(1.0)
  , m_torqueScaleFactor(1.0)
  , m_fx(0.0)
  , m_fy(0.0)
  , m_tz(0.0)
{
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->setZValue(100);

    m_arrowXY->setColor(   QColor(255,127, 39));   // orange
    m_arrowX->setColor(    QColor(237, 28, 36));   // red
    m_arrowY->setColor(    QColor( 34,177, 76));   // green
    m_arcArrowZ->setColor( QColor(  0,162,232));   // blue

    m_labelXY->setBrush(QBrush(QColor(255,127, 39)));   // orange
    m_labelX->setBrush(QBrush( QColor(237, 28, 36)));   // red
    m_labelY->setBrush(QBrush( QColor( 34,177, 76)));   // green
    m_labelZ->setBrush(QBrush( QColor(  0,162,232)));   // blue

    m_labelXY->setVisible(m_labelVisible);
    m_labelX->setVisible( m_labelVisible);
    m_labelY->setVisible( m_labelVisible);
    m_labelZ->setVisible( m_labelVisible);
}

bool TensorItem::isArrowInverted() const
{
    return m_arrowInverted;
}

void TensorItem::setInvertedArrow(bool inverted)
{
    m_arrowInverted = inverted;
    updateArrows();
}

bool TensorItem::isComponentVisible() const
{
    return m_componantVisible;
}

void TensorItem::setComponentVisible(bool visible)
{
    m_componantVisible = visible;
    updateArrows();
}

bool TensorItem::isResultantVisible() const
{
    return m_resultantVisible;
}

void TensorItem::setResultantVisible(bool visible)
{
    m_resultantVisible = visible;
    updateArrows();
}

bool TensorItem::isTorqueVisible() const
{
    return m_torqueVisible;
}

void TensorItem::setTorqueVisible(bool visible)
{
    m_torqueVisible = visible;
    updateArrows();
}

bool TensorItem::isLabelVisible() const
{
    return m_labelVisible;
}

void TensorItem::setLabelVisible(bool visible)
{
    m_labelVisible = visible;
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

    /* Arrows */
    m_arrowXY->setVisible(m_resultantVisible);
    m_arrowX->setVisible(m_componantVisible);
    m_arrowY->setVisible(m_componantVisible);
    m_arcArrowZ->setVisible(m_torqueVisible);

    /// \todo Use pow(1.000152, factor) instead ?

    /// \todo  m_fx, m_fy... can be 0, NaN of Infinite. What to do in such case ?

    if (m_arrowInverted) {
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


    /* Labels */
    m_labelXY->setVisible(m_resultantVisible && m_labelVisible);
    m_labelX->setVisible( m_componantVisible && m_labelVisible);
    m_labelY->setVisible( m_componantVisible && m_labelVisible);
    m_labelZ->setVisible( m_torqueVisible    && m_labelVisible);

    qreal fxy = qSqrt( qPow(m_fx, 2) + qPow(m_fy, 2));
    m_labelXY->setText(QString("Fxy=%0N").arg(fxy, 0, 'f', 1));
    m_labelX->setText(QString("Fx=%0N").arg(m_fx, 0, 'f', 1));
    m_labelY->setText(QString("Fy=%0N").arg(m_fy, 0, 'f', 1));
    m_labelZ->setText(QString("Tz=%0N.m").arg(m_tz, 0, 'f', 1));

    qreal w = m_labelXY->boundingRect().height() ;
    QPointF offset(C_ARROW_SIZE / 2, -w);
    if (m_arrowInverted) {
        m_labelXY->setPos( m_arrowXY->line().p1() + offset);
        m_labelX->setPos(  m_arrowX->line().p1()  + offset);
        m_labelY->setPos(  m_arrowY->line().p1()  + offset);
    } else {
        m_labelXY->setPos( m_arrowXY->line().p2() + offset);
        m_labelX->setPos(  m_arrowX->line().p2()  + offset);
        m_labelY->setPos(  m_arrowY->line().p2()  + offset);
    }
    m_labelZ->setPos(offset);

    QGraphicsItem::update();
}

