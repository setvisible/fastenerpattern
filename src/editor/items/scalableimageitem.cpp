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

#include "scalableimageitem.h"
#include "scalablepointitem.h"

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsScene>

ScalableImageObject::ScalableImageObject(ScalableImageItem *parent) : QObject(parent)
  , m_parent(parent)
{
}

void ScalableImageObject::onCornerPositionChanged()
{
    ScalablePointItem *item = static_cast<ScalablePointItem *>(sender());
    if (!item)
        return;
    if (!item->isSelected())
        return;
    m_parent->setCorner(item);
}


ScalableImageItem::ScalableImageItem(QGraphicsItem *parent) : QGraphicsObject(parent)
  , m_object(new ScalableImageObject(this))
  , m_rect(QRect())
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setZValue(-1);

    /*  [0]      [1]
     *   +-------+
     *   |       |
     *   +-------+
     *  [2]      [3]
     */
    for (int i = 0 ; i < 4 ; ++i) {
        ScalablePointItem *item = new ScalablePointItem(this);
        m_scalePoints[i] = item;
        QObject::connect(item, SIGNAL(xChanged()), m_object, SLOT(onCornerPositionChanged()));
        QObject::connect(item, SIGNAL(yChanged()), m_object, SLOT(onCornerPositionChanged()));
    }
}

ScalableImageItem::~ScalableImageItem()
{
    if (m_object)
        delete m_object;
}

void ScalableImageItem::setObjectVisible(bool visible)
{
    this->setVisible(visible);
    /// \todo  for (int i = 0 ; i < 4 ; ++i) {
    /// \todo      m_scalePoints[i]->setVisible(visible);
    /// \todo  }
}

QUrl ScalableImageItem::url() const
{
    return m_url;
}
void ScalableImageItem::setUrl(const QUrl &url)
{
    m_url = url;

    QString imagePath = url.path();
    QPixmap image(url.path());
    if (image.isNull()) {
        image.load(imagePath.right(imagePath.size() - 1));
    }
    m_pixmap = image;
    if (!m_pixmap.isNull()) {
        m_rect = m_pixmap.rect();
        m_scalePoints[0]->setPos(QPointF(m_rect.topLeft()));
        m_scalePoints[1]->setPos(QPointF(m_rect.topRight()));
        m_scalePoints[2]->setPos(QPointF(m_rect.bottomLeft()));
        m_scalePoints[3]->setPos(QPointF(m_rect.bottomRight()));
    }
    scene()->update();
}

QRect ScalableImageItem::rect() const
{
    return m_rect;
}
void ScalableImageItem::setRect(const QRect &rect)
{
    if (m_pixmap.height() == 0 || m_pixmap.width() == 0){
        qWarning("ScalableImage::setRect() warning: current pixmap has 0-dimension!");
        return;
    }
    if (rect.width() == 0){
        qWarning("ScalableImage::setRect() warning: given rect has 0-dimension!");
        return;
    }
    qreal imageAR = (qreal)m_pixmap.height() / (qreal)m_pixmap.width();
    qreal rectAR = (qreal)rect.height() / (qreal)rect.width();
    m_rect = rect;
    if (imageAR > rectAR) {
        m_rect.setWidth(rect.height() / imageAR);
    } else {
        m_rect.setHeight(rect.width() * imageAR);
    }
    m_scalePoints[0]->setPos(QPointF(m_rect.topLeft()));
    m_scalePoints[1]->setPos(QPointF(m_rect.topRight()));
    m_scalePoints[2]->setPos(QPointF(m_rect.bottomLeft()));
    m_scalePoints[3]->setPos(QPointF(m_rect.bottomRight()));

    scene()->update();
}


void ScalableImageItem::setCorner(const ScalablePointItem *item)
{
    if (        item == m_scalePoints[0]) { m_rect.setTopLeft(item->pos().toPoint());
    } else if ( item == m_scalePoints[1]) { m_rect.setTopRight(item->pos().toPoint());
    } else if ( item == m_scalePoints[2]) { m_rect.setBottomLeft(item->pos().toPoint());
    } else if ( item == m_scalePoints[3]) { m_rect.setBottomRight(item->pos().toPoint());
    }

    /// \todo Restrict on diagonal to preserve the scale ratio ?
    /// \todo   qreal imageAR = (qreal)m_pixmap.height() / (qreal)m_pixmap.width();
    /// \todo   qreal rectAR = (qreal)m_rect.height() / (qreal)m_rect.width();
    /// \todo   if (imageAR > rectAR) {
    /// \todo       m_rect.setWidth(m_rect.height() / imageAR);
    /// \todo   } else {
    /// \todo       m_rect.setHeight(m_rect.width() * imageAR);
    /// \todo   }

    m_scalePoints[0]->setPos(QPointF(m_rect.topLeft()));
    m_scalePoints[1]->setPos(QPointF(m_rect.topRight()));
    m_scalePoints[2]->setPos(QPointF(m_rect.bottomLeft()));
    m_scalePoints[3]->setPos(QPointF(m_rect.bottomRight()));

    this->scene()->update();
}

QRectF ScalableImageItem::boundingRect() const
{
    return QRectF(m_rect)
            | m_scalePoints[0]->boundingRect() | m_scalePoints[1]->boundingRect()
            | m_scalePoints[2]->boundingRect() | m_scalePoints[3]->boundingRect();
}

void ScalableImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    const bool _isSelected = !m_pixmap.isNull() && isSelected();
    if (!m_pixmap.isNull()) {
        painter->drawPixmap(m_rect, m_pixmap);
        if (_isSelected) {
            painter->setPen(QPen(Qt::blue, 1));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(m_rect);
        }
    }
}
