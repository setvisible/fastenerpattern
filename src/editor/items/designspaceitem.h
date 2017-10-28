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

#ifndef EDITOR_ITEMS_DESIGN_SPACE_ITEM_H
#define EDITOR_ITEMS_DESIGN_SPACE_ITEM_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtWidgets/QGraphicsObject>

class HandleItem;
class QGraphicsPolygonItem;
class DesignSpaceItem;

/******************************************************************************
 ******************************************************************************/
class DesignSpaceObject : public QObject
{
    Q_OBJECT
public:
    explicit DesignSpaceObject(DesignSpaceItem *parent = Q_NULLPTR);

public Q_SLOTS:
    void onCornerPositionChanged();

private:
    DesignSpaceItem *m_parent;
};

/******************************************************************************
 ******************************************************************************/
class DesignSpaceItem : public QGraphicsObject
{
   // friend class DesignSpaceObject;

public:
    explicit DesignSpaceItem(QGraphicsItem *parent = Q_NULLPTR);
    ~DesignSpaceItem();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

    QPolygonF polygon() const;
    void setPolygon(const QPolygonF &polygon);

//protected:
    void setCorner(const HandleItem *item);

private:
    DesignSpaceObject *m_object;
    QGraphicsPolygonItem *m_polygonItem;
    QList<HandleItem*> m_corners;


};

#endif // EDITOR_ITEMS_DESIGN_SPACE_ITEM_H
