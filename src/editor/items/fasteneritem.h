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

#ifndef EDITOR_FASTENER_GRAPHICS_ITEM_H
#define EDITOR_FASTENER_GRAPHICS_ITEM_H

#include <Core/Fastener>
#include <Core/Tensor>
#include <QGraphicsObject>

class TensorItem;
class SymbolItem;
class FastenerItem : public QGraphicsObject
{
    Q_PROPERTY(qreal truePositionX READ truePositionX)
    Q_PROPERTY(qreal truePositionY READ truePositionY)
    Q_PROPERTY(qreal trueDiameter READ trueDiameter WRITE setTrueDiameter)
    Q_PROPERTY(Tensor result READ result WRITE setResult)

public:
    explicit FastenerItem(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

    qreal truePositionX() const;
    qreal truePositionY() const;
    void setTruePosition(qreal x, qreal y);

    qreal trueDiameter() const;
    void setTrueDiameter(qreal trueDiameter);

    Tensor result() const;
    void setResult(const Tensor &tensor);

private:
    TensorItem *m_tensorItem;
    SymbolItem *m_symbolItem;

    QPointF newPos;
    QPointF m_position;
    QPointF m_forceVector;
    QPointF m_originToCenter;

};

#endif // EDITOR_FASTENER_GRAPHICS_ITEM_H
