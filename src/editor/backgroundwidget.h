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
class QGraphicsSimpleTextItem;
class QGraphicsView;

class BackgroundWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal pixelsPerUnit READ pixelsPerUnit WRITE setPixelsPerUnit)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl)

public:
    enum Feature {
        None                = 0,
        HorizontalAxis      = 1,
        VerticalAxis        = 2,
        Grid                = 4,
        Image               = 8,
        DragImageHereText   = 16
    };
    Q_DECLARE_FLAGS(Features, Feature)

    explicit BackgroundWidget(QWidget *parent = 0);

    bool testFlag(Feature feature) const;
    void setFlag(Feature feature, const bool enabled);
    void setFlags(QFlags<Feature> features, const bool enabled);

    QGraphicsScene* scene() const;
    BackgroundView* viewport() const;
    QGraphicsView* view() const;


public Q_SLOTS:
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
    QFlags<Feature> m_enabledFeatures;
    QGraphicsLineItem* m_horizontalAxis;
    QGraphicsLineItem* m_verticalAxis;
    QGraphicsSimpleTextItem* m_dragImageHereText;

    void updateView();

};

Q_DECLARE_OPERATORS_FOR_FLAGS(BackgroundWidget::Features)

#endif // EDITOR_BACKGROUND_WIDGET_H
