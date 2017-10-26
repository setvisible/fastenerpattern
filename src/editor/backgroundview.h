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

#ifndef EDITOR_BACKGROUND_VIEW_H
#define EDITOR_BACKGROUND_VIEW_H

/*
 * Inspired by Tiled (Tile Editor) in /src/tiled/mapview.cpp
 */

#include <QtCore/QUrl>
#include <QtWidgets/QGraphicsView>

class ScalableImageItem;

class BackgroundView : public QGraphicsView
{
    Q_PROPERTY(bool handScrolling READ handScrolling WRITE setHandScrolling)
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible)
    Q_PROPERTY(qreal pxPerUnit READ pixelsPerUnit WRITE setPixelsPerUnit)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl)

public:
    explicit BackgroundView(QGraphicsScene *scene, QWidget *parent = 0);
    ~BackgroundView();

    /* Options */
    bool handScrolling() const;
    bool isGridVisible() const;
    bool isImageVisible() const;
    qreal pixelsPerUnit() const;
    QUrl imageUrl() const;

    void setHandScrolling(bool handScrolling);
    void setGridVisible(bool visible);
    void setImageVisible(bool visible);
    void setPixelsPerUnit(qreal pxPerUnit); /* number of pixels per meter */
    void setImageUrl(const QUrl &url);


protected:
    virtual void hideEvent(QHideEvent *event);

    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    virtual void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;


private:
    bool m_handScrolling;
    bool m_gridVisible;
    qreal m_pxPerUnit;
    ScalableImageItem *m_imageItem;

    QPoint  m_lastMousePos;
    QPointF m_lastMouseScenePos;

    void drawLinesWithInterval(qreal px, QPainter *painter, const QRectF &rect);
};

#endif // EDITOR_BACKGROUND_VIEW_H
