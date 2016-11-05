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

#include <QGraphicsItemGroup>

class Tensor;
class ArrowItem;
class ArcArrowItem;
class TensorItem : public QGraphicsItemGroup
{
    Q_PROPERTY(bool isArrowInverted READ isArrowInverted WRITE setInvertedArrow)
    Q_PROPERTY(bool isComponantVisible READ isComponantVisible WRITE setComponantVisible)
    Q_PROPERTY(bool isResultantVisible READ isResultantVisible WRITE setResultantVisible)
    Q_PROPERTY(bool isTorqueVisible READ isTorqueVisible WRITE setTorqueVisible)
    Q_PROPERTY(qreal loadScaleFactor READ loadScaleFactor WRITE setLoadScaleFactor)
    Q_PROPERTY(qreal torqueScaleFactor READ torqueScaleFactor WRITE setTorqueScaleFactor)
    Q_PROPERTY(Tensor tensor READ tensor WRITE setTensor)

public:
    explicit TensorItem(QGraphicsItem *parent = Q_NULLPTR);

public Q_SLOTS:
    bool isArrowInverted() const;
    bool isComponantVisible() const;
    bool isResultantVisible() const;
    bool isTorqueVisible() const;
    void setInvertedArrow(bool inverted);
    void setComponantVisible(bool visible);
    void setResultantVisible(bool visible);
    void setTorqueVisible(bool visible);

    qreal loadScaleFactor() const;
    qreal torqueScaleFactor() const;
    void setLoadScaleFactor(qreal factor);
    void setTorqueScaleFactor(qreal factor);

    Tensor tensor() const;
    void setTensor(const Tensor &tensor);

protected:
    ArrowItem *m_arrowXY;
    ArrowItem *m_arrowX;
    ArrowItem *m_arrowY;
    ArcArrowItem *m_arcArrowZ;

private:
    bool m_isArrowInverted;
    bool m_isComponantVisible;
    bool m_isResultantVisible;
    bool m_isTorqueVisible;
    double m_loadScaleFactor;
    double m_torqueScaleFactor;
    double m_fx;
    double m_fy;
    double m_tz;

    void updateArrows();
};

#endif // EDITOR_ITEMS_TENSOR_ITEM_H
