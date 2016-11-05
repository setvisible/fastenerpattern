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

#include "backgroundwidget.h"

#include "backgroundview.h"

#include <QLayout>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsLineItem>

/*! \class BackgroundWidget
 *  \brief The class BackgroundWidget manages the graphics scene.
 *
 */

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
  , m_scene(new QGraphicsScene(this))
  , m_view(Q_NULLPTR)
  , m_mainLayout(new QVBoxLayout(this))
  , m_enabledFeatures(Features::None)
  , m_horizontalAxis(Q_NULLPTR)
  , m_verticalAxis(Q_NULLPTR)
{
    /* Create Layout */
    m_view = new BackgroundView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_mainLayout->addWidget(m_view);
    this->setLayout(m_mainLayout);

    /* Create Features */
    m_horizontalAxis = new QGraphicsLineItem(m_scene->sceneRect().left(), 0,
                                             m_scene->sceneRect().right(), 0);
    m_verticalAxis = new QGraphicsLineItem(0, m_scene->sceneRect().bottom(),
                                           0, m_scene->sceneRect().top());
    m_scene->addItem(m_horizontalAxis);
    m_scene->addItem(m_verticalAxis);

    QObject::connect(m_scene, SIGNAL(sceneRectChanged(QRectF)),
                     this, SLOT(onSceneRectChanged(QRectF)));

}

void BackgroundWidget::enableFeatures(QFlags<Features> features)
{
    if ( features.testFlag(Features::HorizontalAxis)
         && !m_enabledFeatures.testFlag(Features::HorizontalAxis)) {
        m_enabledFeatures |= Features::HorizontalAxis;
    }

    if ( features.testFlag(Features::VerticalAxis)
         && !m_enabledFeatures.testFlag(Features::VerticalAxis)) {
        m_enabledFeatures |= Features::VerticalAxis;
    }

    if ( features.testFlag(Features::DragImageHereText)
         && !m_enabledFeatures.testFlag(Features::DragImageHereText)) {
        QFont font;
        font.setWeight(QFont::Light);
        m_scene->addText(tr("Drag an image here"), font);
        m_enabledFeatures |= Features::DragImageHereText;
    }
}


QGraphicsScene* BackgroundWidget::scene() const
{
    return m_scene;
}

BackgroundView* BackgroundWidget::viewport() const
{
    return m_view;
}

QGraphicsView* BackgroundWidget::view() const
{
    return (QGraphicsView*)(m_view);
}

void BackgroundWidget::onSceneRectChanged(const QRectF &rect)
{
    if (m_enabledFeatures.testFlag(Features::HorizontalAxis))
        m_horizontalAxis->setLine(rect.left() + 1, 0, rect.right() - 1, 0);

    if (m_enabledFeatures.testFlag(Features::VerticalAxis))
        m_verticalAxis->setLine(0, rect.bottom() - 1, 0, rect.top() + 1);
}


bool BackgroundWidget::isAxesVisible() const
{
    return m_enabledFeatures.testFlag(Features::HorizontalAxis)
            && m_enabledFeatures.testFlag(Features::VerticalAxis);
}

void BackgroundWidget::setAxesVisible(bool visible)
{
    m_enabledFeatures.setFlag(Features::HorizontalAxis, visible);
    m_enabledFeatures.setFlag(Features::VerticalAxis, visible);
    this->update();
}

bool BackgroundWidget::isGridVisible() const
{
    return m_view->isGridVisible();
}

void BackgroundWidget::setGridVisible(bool visible)
{
    m_view->setGridVisible(visible);
}

bool BackgroundWidget::isImageVisible() const
{
    return m_view->isImageVisible();
}

void BackgroundWidget::setImageVisible(bool visible)
{
    m_view->setImageVisible(visible);
}

qreal BackgroundWidget::pixelsPerUnit() const
{
    return m_view->pixelsPerUnit();
}

void BackgroundWidget::setPixelsPerUnit(qreal pxPerUnit)
{
    m_view->setPixelsPerUnit(pxPerUnit);
}

QUrl BackgroundWidget::imageUrl() const
{
    return m_view->imageUrl();
}

void BackgroundWidget::setImageUrl(const QUrl &url)
{
    m_view->setImageUrl(url);
}

