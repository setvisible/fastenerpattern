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

#ifndef EDITOR_ITEMS_MEASUREITEM_H
#define EDITOR_ITEMS_MEASUREITEM_H

#include <QtWidgets/QGraphicsLineItem>

class QGraphicsSimpleTextItem;
class MeasureItem : public QGraphicsLineItem
{
public:
    explicit MeasureItem(QGraphicsItem *parent = Q_NULLPTR);

    qreal endSpace() const;
    void setEndSpace(qreal space);

    QColor color() const;
    void setColor(const QColor &color);

    QString text() const;
    void setText(const QString &text);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

private:
    qreal m_endSpace;
    QColor m_color;
    QGraphicsSimpleTextItem *m_label;
};


#endif // EDITOR_ITEMS_MEASUREITEM_H
