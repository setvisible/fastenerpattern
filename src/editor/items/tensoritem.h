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

#ifndef EDITOR_ITEMS_TENSOR_ITEM_H
#define EDITOR_ITEMS_TENSOR_ITEM_H

#include <QtWidgets/QGraphicsItemGroup>

class Tensor;
class ArrowItem;
class ArcArrowItem;
class QGraphicsSimpleTextItem;
class TensorItem : public QGraphicsItemGroup
{
    Q_PROPERTY(bool arrowInverted READ isArrowInverted WRITE setInvertedArrow)
    Q_PROPERTY(bool componentVisible READ isComponentVisible WRITE setComponentVisible)
    Q_PROPERTY(bool resultantVisible READ isResultantVisible WRITE setResultantVisible)
    Q_PROPERTY(bool torqueVisible READ isTorqueVisible WRITE setTorqueVisible)
    Q_PROPERTY(bool labelVisible READ isLabelVisible WRITE setLabelVisible)
    Q_PROPERTY(qreal loadScaleFactor READ loadScaleFactor WRITE setLoadScaleFactor)
    Q_PROPERTY(qreal torqueScaleFactor READ torqueScaleFactor WRITE setTorqueScaleFactor)
    Q_PROPERTY(Tensor tensor READ tensor WRITE setTensor)

public:
    explicit TensorItem(QGraphicsItem *parent = Q_NULLPTR);

public Q_SLOTS:
    bool isArrowInverted() const;
    void setInvertedArrow(bool inverted);

    bool isComponentVisible() const;
    void setComponentVisible(bool visible);

    bool isResultantVisible() const;
    void setResultantVisible(bool visible);

    bool isTorqueVisible() const;
    void setTorqueVisible(bool visible);

    bool isLabelVisible() const;
    void setLabelVisible(bool visible);

    qreal loadScaleFactor() const;
    void setLoadScaleFactor(qreal factor);

    qreal torqueScaleFactor() const;
    void setTorqueScaleFactor(qreal factor);

    Tensor tensor() const;
    void setTensor(const Tensor &tensor);

protected:
    ArrowItem *m_arrowXY;
    ArrowItem *m_arrowX;
    ArrowItem *m_arrowY;
    ArcArrowItem *m_arcArrowZ;
    QGraphicsSimpleTextItem *m_labelXY;
    QGraphicsSimpleTextItem *m_labelX;
    QGraphicsSimpleTextItem *m_labelY;
    QGraphicsSimpleTextItem *m_labelZ;

private:
    bool m_arrowInverted;
    bool m_componantVisible;
    bool m_resultantVisible;
    bool m_torqueVisible;
    bool m_labelVisible;
    qreal m_loadScaleFactor;
    qreal m_torqueScaleFactor;
    qreal m_fx;
    qreal m_fy;
    qreal m_tz;

    void updateArrows();
};

#endif // EDITOR_ITEMS_TENSOR_ITEM_H
