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
#include <QGraphicsTextItem>

/*! \class BackgroundWidget
 *  \brief The class BackgroundWidget manages the graphics scene.
 *
 */

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
  , m_scene(new QGraphicsScene(this))
  , m_view(Q_NULLPTR)
  , m_mainLayout(new QVBoxLayout(this))
  , m_enabledFeatures(Feature::None)
  , m_horizontalAxis(Q_NULLPTR)
  , m_verticalAxis(Q_NULLPTR)
  , m_dragImageHereText(Q_NULLPTR)
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

    QFont font;
    font.setWeight(QFont::Light);
    m_dragImageHereText = m_scene->addText(tr("Drag an image here"), font);

    /* Signals / Slots */
    QObject::connect(m_scene, SIGNAL(sceneRectChanged(QRectF)),
                     this, SLOT(onSceneRectChanged(QRectF)));

    this->updateView();

}

/******************************************************************************
 ******************************************************************************/
bool BackgroundWidget::testFlag(Feature feature) const
{
    return m_enabledFeatures.testFlag(feature);
}

void BackgroundWidget::setFlag(Feature feature, const bool enabled)
{
    m_enabledFeatures.setFlag(feature, enabled);
    this->updateView();
}

void BackgroundWidget::setFlags(QFlags<Feature> features, const bool enabled)
{
    if (enabled) {
        m_enabledFeatures |= features;
    } else {
        m_enabledFeatures ^= features;
    }
    this->updateView();
}

/******************************************************************************
 ******************************************************************************/
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

/******************************************************************************
 ******************************************************************************/
void BackgroundWidget::onSceneRectChanged(const QRectF &rect)
{
    m_horizontalAxis->setLine(rect.left() + 1, 0, rect.right() - 1, 0);
    m_verticalAxis->setLine(0, rect.bottom() - 1, 0, rect.top() + 1);
    this->updateView();
}

/******************************************************************************
 ******************************************************************************/
void BackgroundWidget::updateView()
{
    m_horizontalAxis->setVisible(m_enabledFeatures.testFlag(Feature::HorizontalAxis));
    m_verticalAxis->setVisible(m_enabledFeatures.testFlag(Feature::VerticalAxis));
    m_view->setGridVisible(m_enabledFeatures.testFlag(Feature::Grid));
    m_view->setImageVisible(m_enabledFeatures.testFlag(Feature::Image));
    m_dragImageHereText->setVisible(m_enabledFeatures.testFlag(Feature::DragImageHereText));
}

/******************************************************************************
 ******************************************************************************/
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

