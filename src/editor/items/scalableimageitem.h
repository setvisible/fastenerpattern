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

#ifndef EDITOR_ITEMS_SCALABLE_IMAGE_ITEM_H
#define EDITOR_ITEMS_SCALABLE_IMAGE_ITEM_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsObject>

class ScalablePointItem;
class ScalableImageItem;
class ScalableImageObject : public QObject
{
    Q_OBJECT
public:
    explicit ScalableImageObject(ScalableImageItem *parent = Q_NULLPTR);

public Q_SLOTS:
    void onCornerPositionChanged();
private:
    ScalableImageItem *m_parent;
};

class ScalableImageItem : public QGraphicsObject
{
public:
    explicit ScalableImageItem(QGraphicsItem *parent = Q_NULLPTR);
    ~ScalableImageItem();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

    void setObjectVisible(bool visible);

    QUrl url() const;
    void setUrl(const QUrl &url);
    QRect rect() const;
    void setRect(const QRect &rect);

    void setCorner(const ScalablePointItem *item);


private:
    ScalableImageObject *m_object;
    ScalablePointItem *m_scalePoints[4];
    QPixmap m_pixmap;
    QRect m_rect;
    QUrl m_url;


};

#endif // EDITOR_ITEMS_SCALABLE_IMAGE_ITEM_H
