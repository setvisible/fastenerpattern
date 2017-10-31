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

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtGui/QCursor>
#include <QtGui/QPainter>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsPolygonItem>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtGui/QContextMenuEvent>



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

void DesignSpaceObject::addHandle()
{
    const QAction *action = qobject_cast<const QAction *>(this->sender());
    const uint item = action->data().toUInt();
    qDebug() << Q_FUNC_INFO << item;
}

void DesignSpaceObject::removeHandle()
{
    const QAction *action = qobject_cast<const QAction *>(this->sender());
    const uint handlePointer = action->data().toUInt();


    qDebug() << Q_FUNC_INFO << handlePointer;

}

/******************************************************************************
 ******************************************************************************/
class BorderItem : public QGraphicsObject
{
public:
    explicit BorderItem(QGraphicsPolygonItem *polygon, QGraphicsItem *parent = Q_NULLPTR)
        : QGraphicsObject(parent)
        , m_polygonItem(polygon)
    {
    }

    QRectF boundingRect() const Q_DECL_OVERRIDE
    {
        return shape().boundingRect();
    }
    QPainterPath shape() const Q_DECL_OVERRIDE
    {
        QPainterPath path;
        path.addPolygon(m_polygonItem->polygon());
        path.closeSubpath();
        QPainterPathStroker stroker;
        stroker.setWidth(C_ARROW_SIZE);
        return stroker.createStroke(path);
    }
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE
    {
    }

private:
    QGraphicsPolygonItem *m_polygonItem;
};

/******************************************************************************
 ******************************************************************************/
DesignSpaceItem::DesignSpaceItem(QGraphicsItem *parent) : QGraphicsObject(parent)
  , m_object(new DesignSpaceObject(this))
  , m_polygonItem(new QGraphicsPolygonItem(this))
  , m_borderItem(new BorderItem(m_polygonItem, this))
  , m_brush(QBrush(QColor(153, 217, 234, 126))) /* cyan */
  , m_brushSelected(QBrush(QColor(217, 153, 234, 126))) /* pink */
{
    /***********************************************************************\
    *  Remarks:                                                             *
    *                                                                       *
    *  m_object                                                             *
    *      this object is used for the callbacks of the Handles             *
    *      (corners of the polygon)                                         *
    *                                                                       *
    *  m_borderItem                                                         *
    *      this is a transparent (but visible!) graphics object             *
    *      that catches the mouse click events occuring                     *
    *      near the polygon's border.                                       *
    *      Internally, the BorderItem simulates a thick enclosed path.      *
    *                                                                       *
    *  m_polygonItem                                                        *
    *      this is a colored fill polygon representing the                  *
    *      area of the design space. It catches the composite object's      *
    *      mouse events                                                     *
    *                                                                       *
    \***********************************************************************/

    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setZValue(-50); /* Below the axes, but above the BG image and the grid */

    m_polygonItem->setFillRule(Qt::WindingFill);
    m_polygonItem->setPen(Qt::NoPen);


    m_borderItem->installEventFilter(this);
    m_borderItem->setVisible(true);

}

DesignSpaceItem::~DesignSpaceItem()
{
    if (m_object)
        delete m_object;
}

/******************************************************************************
 ******************************************************************************/
bool DesignSpaceItem::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneContextMenu) {
        QGraphicsSceneContextMenuEvent *ctxtEvent
                = static_cast<QGraphicsSceneContextMenuEvent*>(event);
        {
            HandleItem *handle = dynamic_cast<HandleItem*>(obj);
            if (handle) {
                QMenu menu;
                QAction *action = menu.addAction(
                            tr("Remove Handle"), m_object, SLOT(removeHandle()));
                action->setData( (uint)handle );
                menu.exec(ctxtEvent->screenPos());
                return true;
            }
        }
        {
            BorderItem *border = dynamic_cast<BorderItem*>(obj);
            if (border) {
                QMenu menu;
                QAction *action = menu.addAction(
                            tr("Add Handle"), m_object, SLOT(addHandle()));
                action->setData( (uint)border );
                menu.exec(ctxtEvent->screenPos());
                return true;
            }
        }

    }
    /* Pass the event on to the parent class */
    return QObject::eventFilter(obj, event);

}

/******************************************************************************
 ******************************************************************************/

QRectF DesignSpaceItem::boundingRect() const
{
    QRectF rect = m_polygonItem->boundingRect();
    foreach (auto corner, m_corners) {
        rect |= corner->boundingRect();
    }
    rect |= m_borderItem->boundingRect();
    return rect;
}

QPainterPath DesignSpaceItem::shape() const
{
    QPainterPath polygonPath;
    polygonPath.addPolygon(m_polygonItem->polygon());
    QPainterPath borderPath = m_borderItem->shape();
    QPainterPath path = borderPath.united(polygonPath);
    return path;
}

void DesignSpaceItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
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

    m_borderItem->setVisible( selected );
    this->setFlag(QGraphicsItem::ItemIsMovable, selected);
    this->setCursor( selected ? Qt::SizeAllCursor : Qt::ArrowCursor );

    if (selected) {
        m_polygonItem->setBrush(m_brushSelected);
    } else {
        m_polygonItem->setBrush(m_brush);
    }
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
        item->setCoordinateVisible(true);
        QObject::connect(item, SIGNAL(xChanged()), m_object, SLOT(onCornerPositionChanged()));
        QObject::connect(item, SIGNAL(yChanged()), m_object, SLOT(onCornerPositionChanged()));
        item->installEventFilter(this);
        m_corners << item;
    }
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
            m_polygonItem->setPolygon(polygon);
            break;
        }
    }
}
