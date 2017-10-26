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

#ifndef EDITOR_ITEMS_ARCARROWITEM_H
#define EDITOR_ITEMS_ARCARROWITEM_H

#include <QtWidgets/QGraphicsEllipseItem>

class ArcArrowItem : public QGraphicsEllipseItem
{
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal diameter READ diameter WRITE setDiameter)

public:
    explicit ArcArrowItem(QGraphicsItem *parent = Q_NULLPTR);

    QColor color() const;
    void setColor(const QColor &color);

    qreal diameter() const;
    void setDiameter(qreal diameter);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

private:
    QColor m_color;
    qreal m_diameter;

};

#endif // EDITOR_ITEMS_ARCARROWITEM_H
