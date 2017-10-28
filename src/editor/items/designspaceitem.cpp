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

#include "designspaceitem.h"

#include "handleitem.h"
#include "utils_scale.h"

#include <QtCore/QDebug>
#include <QtGui/QCursor>
#include <QtGui/QPainter>
//#include <QtGui/QPen>
#include <QtWidgets/QGraphicsPolygonItem>

#include <QtWidgets/QGraphicsScene>

/******************************************************************************
 ******************************************************************************/
DesignSpaceObject::DesignSpaceObject(DesignSpaceItem *parent) : QObject(parent)
  , m_parent(parent)
{
}

void DesignSpaceObject::onCornerPositionChanged()
{
    HandleItem *item = static_cast<HandleItem *>(sender());
    if (!item)
        return;
    if (!item->isSelected())
        return;
    m_parent->setCorner(item);
}

/******************************************************************************
 ******************************************************************************/
DesignSpaceItem::DesignSpaceItem(QGraphicsItem *parent) : QGraphicsObject(parent)
  , m_object(new DesignSpaceObject(this))
  , m_polygonItem(new QGraphicsPolygonItem(this))
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);

    m_polygonItem->setCursor(Qt::SizeAllCursor);
    m_polygonItem->setZValue(50);
}

DesignSpaceItem::~DesignSpaceItem()
{
    if (m_object)
        delete m_object;
}

QRectF DesignSpaceItem::boundingRect() const
{
    return m_polygonItem->boundingRect();
//    return QRectF(m_rect)
//            | m_scalePoints[0]->boundingRect() | m_scalePoints[1]->boundingRect()
//            | m_scalePoints[2]->boundingRect() | m_scalePoints[3]->boundingRect();
}

QPainterPath DesignSpaceItem::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygonItem->polygon());
 //   path.addRect(m_polygonItem->boundingRect());
 return path;

  //  if (line().length() > 0.0) {
        QPainterPathStroker stroker;
        path.addPolygon(m_polygonItem->polygon());
        stroker.setWidth(C_ARROW_SIZE);
        return stroker.createStroke(path);
  //  }

}

void DesignSpaceItem::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem *, QWidget *)
{
    bool selected = isSelected();
    if (!selected) {
        foreach (auto corner, m_corners) {
            if (corner->isSelected()) {
                selected = true;
                break;
            }
        }
    }
    foreach (auto corner, m_corners) {
        corner->setVisible( selected );
    }


   // painter->drawPolygon(polygon());
   // m_polygonItem->paint(painter);
            //const bool _isSelected = !m_pixmap.isNull() && isSelected();
    //if (!m_pixmap.isNull()) {
    //    painter->drawPixmap(m_rect, m_pixmap);
    //    if (_isSelected) {
    //        painter->setPen(QPen(Qt::blue, 1));
    //        painter->setBrush(Qt::NoBrush);
    //        painter->drawRect(m_rect);
    //    }
    //}
}


QPolygonF DesignSpaceItem::polygon() const
{
    return m_polygonItem->polygon();
}

void DesignSpaceItem::setPolygon(const QPolygonF &polygon)
{
    m_polygonItem->setPolygon(polygon);

    m_corners.clear();
    foreach (const QPointF &point, polygon) {
        HandleItem *item = new HandleItem(this);
        item->setPos( point );
        QObject::connect(item, SIGNAL(xChanged()), m_object, SLOT(onCornerPositionChanged()));
        QObject::connect(item, SIGNAL(yChanged()), m_object, SLOT(onCornerPositionChanged()));
        m_corners << item;
    }

    // this->scene()->update();
}

void DesignSpaceItem::setCorner(const HandleItem *item)
{
    QPolygonF polygon = m_polygonItem->polygon();
    Q_ASSERT(polygon.count() ==  m_corners.count());

    for (int i = 0; i < m_corners.count(); ++i) {
        if (m_corners.at(i) == item) {
            QPointF point = polygon.at(i);
            point.setX(item->x());
            point.setY(item->y());
            polygon.replace(i,point);
            m_polygonItem->setPolygon( polygon );
            break;
        }
    }
  //  this->scene()->update();
}
