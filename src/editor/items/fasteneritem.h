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
#include <QtWidgets/QGraphicsObject>

class TensorItem;
class SymbolItem;
class FastenerItem : public QGraphicsObject
{
    Q_PROPERTY(qreal positionXInMeter READ positionXInMeter)
    Q_PROPERTY(qreal positionYInMeter READ positionYInMeter)
    Q_PROPERTY(qreal diameterInMeter READ diameterInMeter WRITE setDiameterInMeter)
    Q_PROPERTY(Tensor result READ result WRITE setResult)
    Q_PROPERTY(bool componentVisible READ isComponentVisible WRITE setComponentVisible)
    Q_PROPERTY(bool resultantVisible READ isResultantVisible WRITE setResultantVisible)
    Q_PROPERTY(bool torqueVisible READ isTorqueVisible WRITE setTorqueVisible)
    Q_PROPERTY(bool labelVisible READ isLabelVisible WRITE setLabelVisible)

public:
    explicit FastenerItem(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

    qreal positionXInMeter() const;
    qreal positionYInMeter() const;
    void setPositionInMeter(qreal x_in_meter, qreal y_in_meter);

    qreal diameterInMeter() const;
    void setDiameterInMeter(qreal diameter_in_meter);

    Tensor result() const;
    void setResult(const Tensor &tensor);

    bool isComponentVisible() const;
    void setComponentVisible(bool visible);

    bool isResultantVisible() const;
    void setResultantVisible(bool visible);

    bool isTorqueVisible() const;
    void setTorqueVisible(bool visible);

    bool isLabelVisible() const;
    void setLabelVisible(bool visible);

private:
    TensorItem *m_tensorItem;
    SymbolItem *m_symbolItem;

};

#endif // EDITOR_FASTENER_GRAPHICS_ITEM_H
