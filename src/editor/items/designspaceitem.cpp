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
/*! \brief Return the squared distance between a given point \a point
 *         and a given line defined by its two end-points \a p1 and \a p2.
 */
static qreal squaredDistanceFromLine(const QPointF &point,
                                     const QPointF &p1, const QPointF &p2)
{
    /******************************************************\
    *                          ->                          *
    *                p1        u     p2                    *
    *                 o-----+--------o                     *
    *                  \    |                              *
    *                   \   | ->                           *
    *                 -> \  | w                            *
    *                 v   \ |                              *
    *                      \|                              *
    *                       o point                        *
    * Formula:                                             *
    *     ->   ->         -> ->    ->     ->     ->        *
    *     w  = v  - ( dot(u, v) / (|u| * |u|)) * u         *
    *                                                      *
    \******************************************************/
    QPointF u = p2 - p1;
    QPointF v = point - p1;
    qreal dot = QPointF::dotProduct(u, v);
    qreal squared_u = ( u.x() * u.x() ) + ( u.y() * u.y() );
    QPointF w = v - ( dot / squared_u ) * u;
    qreal squared_distance = ( w.x() * w.x() ) + ( w.y() * w.y() );
    return squared_distance;
}

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
    m_parent->updatePolygonCorner(item);
}

void DesignSpaceObject::addHandle()
{
    /*************************************************************************\
    * - Project the event point on each segment of the polygon.               *
    * - Then, calculate the distance between the event point and              *
    *   the projected point.                                                  *
    * - Finally, take the smallest distance, and insert the projected point   *
    *   to the polygon.                                                       *
    \*************************************************************************/
    const QAction *action = qobject_cast<const QAction *>(this->sender());
    const QPointF pos = action->data().toPointF();

    QPolygonF polygon = m_parent->polygon();

    int final = -1;
    qreal current = 10000 * C_ARROW_SIZE * C_ARROW_SIZE;

    /* Process all the segments from first() to last() */
    for (int i = 1; i < polygon.count(); ++i) {
        const QPointF p1 = polygon.at(i-1);
        const QPointF p2 = polygon.at(i);
        qreal dist = squaredDistanceFromLine(pos, p1, p2);
        if (dist < current) {
            current = dist;
            final = i;
        }
    }
    /* Enclose the loop: process last segment from last() to first() */
    {
        const QPointF p1 = polygon.last();
        const QPointF p2 = polygon.first();
        qreal dist = squaredDistanceFromLine(pos, p1, p2);
        if (dist < current) {
            current = dist;
            final = 0;
        }
    }

    polygon.insert(final, pos);
    m_parent->setPolygon(polygon);
    m_parent->setSelected(true);
}

void DesignSpaceObject::removeHandle()
{
    const QAction *action = qobject_cast<const QAction *>(this->sender());
    const QPointF pos = action->data().toPointF();
    QPolygonF polygon = m_parent->polygon();
    if (polygon.count() < 4) { /* The polygon must have 3 points at least. */
        return;
    }
    for (int i = 0; i < polygon.count(); ++i) {
        if (pos == polygon.at(i)) {
            polygon.removeAt(i);
            m_parent->setPolygon(polygon);
            break;
        }
    }
    m_parent->setSelected(true);
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
  , m_name(QString())
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
    *      Internally, the BorderItem simulates a thick enclosed path,      *
    *      to catch the mouse events, even if the cursor is not             *
    *      -but roughly- on the border.                                     *
    *                                                                       *
    *  m_polygonItem                                                        *
    *      this is a colored fill polygon representing the                  *
    *      area of the design space.                                        *
    *      It also catches the composite object's mouse events,             *
    *      i.e. global displacement of the object.                          *
    *                                                                       *
    \***********************************************************************/

    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setZValue(-50); /* Below the axes, but above the BG image and the grid */

    m_polygonItem->setFillRule(Qt::WindingFill);
    m_polygonItem->setPen(Qt::NoPen);


    m_borderItem->installEventFilter(this);
    m_borderItem->setVisible(true);

    QObject::connect(this, SIGNAL(xChanged()), SIGNAL(changed()));
    QObject::connect(this, SIGNAL(yChanged()), SIGNAL(changed()));
}

DesignSpaceItem::~DesignSpaceItem()
{
    // QObject::disconnect(this, SIGNAL(xChanged()), SIGNAL(changed()));
    // QObject::disconnect(this, SIGNAL(yChanged()), SIGNAL(changed()));

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
                action->setData( handle->scenePos() );
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
                action->setData( ctxtEvent->scenePos() );
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
    foreach (auto corner, m_handleItems) {
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
        foreach (auto corner, m_handleItems) {
            if (corner->isSelected()) {
                selected = true;
                break;
            }
        }
    }
    foreach (auto corner, m_handleItems) {
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

/******************************************************************************
 ******************************************************************************/
QString DesignSpaceItem::name() const
{
    return m_name;
}

void DesignSpaceItem::setName(const QString &name)
{
    if (m_name == name) return;
    m_name = name;
    emit changed();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the item's polygon, or an empty polygon
 * if no polygon has been set.
 *
 * \remark The polygon is given in meters, instead of pixels.
 * To get the polygon in pixel unit, use polygon().
 *
 * \sa setPolygonInMeter(), polygon()
 */
QPolygonF DesignSpaceItem::polygonInMeter() const
{
    const QPolygonF polygon = this->polygon();
    QPolygonF polygonInMeter;
    for (int i = 0; i < polygon.count(); ++i) {
        QPointF pointInMeter = polygon.at(i) / (C_DEFAULT_SCREEN_DPI * 1000.);
        pointInMeter.setY( -1 * pointInMeter.y() ); /* invert the Y */
        polygonInMeter << pointInMeter;
    }
    return polygonInMeter;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Sets the item's polygon to be the given \a polygonInMeter.
 *
 * \remark The polygon is given in meters, instead of pixels.
 * To set the polygon in pixel unit, use setPolygon().
 *
 * \sa polygonInMeter(), setPolygon()
 */
void DesignSpaceItem::setPolygonInMeter(const QPolygonF &polygonInMeter)
{
    QPolygonF polygonInPixel;
    for (int i = 0; i < polygonInMeter.count(); ++i) {
        QPointF pointInPixel = polygonInMeter.at(i) * (C_DEFAULT_SCREEN_DPI * 1000.);
        pointInPixel.setY( -1 * pointInPixel.y() ); /* invert the Y */
        polygonInPixel << pointInPixel;
    }
    this->setPolygon(polygonInPixel);
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the item's polygon, or an empty polygon
 * if no polygon has been set.
 *
 * \remark The polygon is given in pixel, in the global coordinates of the scene.
 */
QPolygonF DesignSpaceItem::polygon() const
{
    QPolygonF polygon = m_polygonItem->polygon();
    polygon.translate(this->pos());
    return polygon;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Sets the item's polygon to be the given \a polygon.
 *
 * \remark The polygon is given in pixel, in the global coordinates of the scene.
 */
void DesignSpaceItem::setPolygon(const QPolygonF &polygon)
{
    if (polygon == this->polygon())
        return;

    const QPointF position = this->pos();
    const QPolygonF trans = polygon.translated( -position );
    m_polygonItem->setPolygon(trans);

    if ( trans.count() != m_handleItems.count() ) {  /* Reallocate properly the HandleItems */

        /* Destruction */
        while (m_handleItems.count() > 0) {
            HandleItem *item = m_handleItems.takeFirst();
            QObject::disconnect(item, SIGNAL(xChanged()), m_object, SLOT(onCornerPositionChanged()));
            QObject::disconnect(item, SIGNAL(yChanged()), m_object, SLOT(onCornerPositionChanged()));
            item->removeEventFilter(this);
            delete item;
        }

        /* Construction */
        foreach (const QPointF &point, trans) {
            HandleItem *item = new HandleItem(this);
            item->setPos( point );
            item->setCoordinateVisible(true);
            QObject::connect(item, SIGNAL(xChanged()), m_object, SLOT(onCornerPositionChanged()));
            QObject::connect(item, SIGNAL(yChanged()), m_object, SLOT(onCornerPositionChanged()));
            item->installEventFilter(this);
            m_handleItems << item;
        }
    }
    Q_ASSERT(trans.count() ==  m_handleItems.count());

    /* Update the position of the Handles */
    for (int i = 0; i < m_handleItems.count(); ++i) {
        m_handleItems[i]->setPos(trans.at(i));
    }

    emit changed();
}

/******************************************************************************
 ******************************************************************************/
/// \internal
void DesignSpaceItem::updatePolygonCorner(const HandleItem *item)
{
    /********************************************************************\
    * Update the m_polygonItem's polygon, but not the Handles positions. *
    * The Handles are already updated internally by the QGraphicsItem.   *
    * This trick avoids to re-emit the x-y-Changed() signals.            *
    \********************************************************************/
    QPolygonF polygon = m_polygonItem->polygon();
    Q_ASSERT(polygon.count() ==  m_handleItems.count());
    for (int i = 0; i < m_handleItems.count(); ++i) {
        if (m_handleItems.at(i) == item) {
            polygon.replace(i, item->pos());
            m_polygonItem->setPolygon(polygon);
            break;
        }
    }
    emit changed();
}
