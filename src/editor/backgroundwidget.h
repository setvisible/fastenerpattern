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

#ifndef EDITOR_BACKGROUND_WIDGET_H
#define EDITOR_BACKGROUND_WIDGET_H

#include <QWidget>
#include <memory>
#include <QFlags>
#include <QUrl>

class BackgroundView;
class QGraphicsScene;
class QVBoxLayout;
class QGraphicsItem;
class QGraphicsLineItem;
class QGraphicsView;

class BackgroundWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool axesVisible READ isAxesVisible WRITE setAxesVisible)
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible)
    Q_PROPERTY(qreal pixelsPerUnit READ pixelsPerUnit WRITE setPixelsPerUnit)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl)

public:
    enum Features {
        None                = 0x0,
        HorizontalAxis      = 0x1,
        VerticalAxis        = 0x2,
        DragImageHereText   = 0x4
    };

    explicit BackgroundWidget(QWidget *parent = 0);

    void enableFeatures(QFlags<Features> features);

    QGraphicsScene* scene() const;
    BackgroundView* viewport() const;
    QGraphicsView* view() const;


public Q_SLOTS:
    bool isAxesVisible() const;
    void setAxesVisible(bool visible);

    bool isGridVisible() const;
    void setGridVisible(bool visible);

    bool isImageVisible() const;
    void setImageVisible(bool visible);

    qreal pixelsPerUnit() const;
    void setPixelsPerUnit(qreal pxPerUnit);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

private Q_SLOTS:
     void onSceneRectChanged( const QRectF & rect );

private:
    QGraphicsScene* m_scene;
    BackgroundView* m_view;
    QVBoxLayout* m_mainLayout;
    QFlags<Features> m_enabledFeatures;
    QGraphicsLineItem* m_horizontalAxis;
    QGraphicsLineItem* m_verticalAxis;

};

#endif // EDITOR_BACKGROUND_WIDGET_H
