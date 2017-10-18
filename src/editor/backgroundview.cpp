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

#include "backgroundview.h"

#include "flexiblescrollbar.h"
#include "items/scalableimageitem.h"
#include "items/utils_scale.h"

#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QtMath>             /* qPow() */
#include <QtGui/QDragMoveEvent>
#include <QtWidgets/QApplication>

/*!
 * \class BackgroundView
 * \brief The class BackgroundView inherits from QGraphicsView,
 * and contains graphic elements that are permanent, i.e. that appear
 * even if the QGraphicsScene is empty (ex: grid, background image)
 *
 * \remark Non-permanent elements are managed by the QGraphicsScene.
 *
 * \remark However Horizontal and Vertical Axes are not in the class
 * BackgroundView, but they are managed by the class BackgroundWidget instead.
 * That is because the Axes are QGraphicsItem, so they are managed by the QGraphicsScene.
 * Moreover we don't want to call the QGraphicsScene from the class BackgroundView.
 *
 * \sa BackgroundWidget
 */
BackgroundView::BackgroundView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
  , m_handScrolling(false)
  , m_gridVisible(true)
  , m_pxPerUnit(10)
  , m_imageItem(Q_NULLPTR)
{
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->viewport()->setMouseTracking(true);
    this->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);

    this->setVerticalScrollBar(new FlexibleScrollBar(Qt::Vertical, this));
    this->setHorizontalScrollBar(new FlexibleScrollBar(Qt::Horizontal, this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    /* Background Image */
    m_imageItem = new ScalableImageItem();
    m_imageItem->setObjectVisible(false);
    scene->addItem(m_imageItem);
}

BackgroundView::~BackgroundView()
{
    setHandScrolling(false); /* Just in case we didn't get a hide event */
}

/* ***************************************************************************
 * ***************************************************************************** */
bool BackgroundView::handScrolling() const
{
    return m_handScrolling;
}

void BackgroundView::setHandScrolling(bool handScrolling)
{
    if (m_handScrolling == handScrolling)
        return;

    m_handScrolling = handScrolling;
    setInteractive(!m_handScrolling);

    if (m_handScrolling) {
        m_lastMousePos = QCursor::pos();
        QApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
        viewport()->grabMouse();
    } else {
        viewport()->releaseMouse();
        QApplication::restoreOverrideCursor();
    }
}

bool BackgroundView::isGridVisible() const
{
    return m_gridVisible;
}

void BackgroundView::setGridVisible(bool visible)
{
    m_gridVisible = visible;
    viewport()->update();
}

qreal BackgroundView::pixelsPerUnit() const
{
    return m_pxPerUnit;
}

void BackgroundView::setPixelsPerUnit(qreal pxPerUnit)
{
    if (pxPerUnit == 0)
        m_pxPerUnit = 10;
    else
        m_pxPerUnit = pxPerUnit;

    scene()->update();
}

bool BackgroundView::isImageVisible() const
{
    Q_ASSERT(m_imageItem);
    return m_imageItem->isVisible();
}

void BackgroundView::setImageVisible(bool visible)
{
    Q_ASSERT(m_imageItem);
    m_imageItem->setVisible(visible);
}

QUrl BackgroundView::imageUrl() const
{
    Q_ASSERT(m_imageItem);
    return m_imageItem->url();
}

void BackgroundView::setImageUrl(const QUrl &url)
{
    Q_ASSERT(m_imageItem);
    QRect position = this->rect();
    position.setHeight(position.height() * 0.9);
    position.translate(0, -position.height());
    m_imageItem->setUrl(url);
    m_imageItem->setVisible( !url.isEmpty() );
}

/* ***************************************************************************
 * ***************************************************************************** */
void BackgroundView::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!m_gridVisible)
        return;

    qreal unitSize = m_pxPerUnit;
    /// \todo Find a suitable unitSize:
    /// \todo qreal unitSize = m_pxPerUnit/100;
    /// \todo while (unitSize < MIN_UNIT_SIZE)
    /// \todo     unitSize *= 10;

    painter->setPen(QPen(QColor(0, 0, 0, 50), C_BACKGROUND_PEN_WIDTH, Qt::SolidLine));
    drawLinesWithInterval(unitSize, painter, rect);

    painter->setPen(QPen(QColor(0, 0, 0, 100), C_BACKGROUND_PEN_WIDTH, Qt::SolidLine));
    drawLinesWithInterval(unitSize * C_BACKGROUND_MINOR_LINES, painter, rect);
}

void BackgroundView::drawLinesWithInterval(qreal px, QPainter *painter, const QRectF &rect)
{
    QVector<QPointF> pointPairs;
    QPointF bottomLeft = rect.bottomLeft();
    QPointF topRight = rect.topRight();
    for (qreal h = 0; h > topRight.y(); h -= px) {
        pointPairs.append(QPointF(bottomLeft.x(), h));
        pointPairs.append(QPointF(topRight.x(), h));
    }
    for (qreal h = px; h < bottomLeft.y(); h += px) {
        pointPairs.append(QPointF(bottomLeft.x(), h));
        pointPairs.append(QPointF(topRight.x(), h));
    }
    for (qreal w = 0; w < topRight.x(); w += px) {
        pointPairs.append(QPointF(w, bottomLeft.y()));
        pointPairs.append(QPointF(w, topRight.y()));
    }
    for (qreal w = -px; w > bottomLeft.x(); w -= px) {
        pointPairs.append(QPointF(w, bottomLeft.y()));
        pointPairs.append(QPointF(w, topRight.y()));
    }
    painter->drawLines(pointPairs);
}


void BackgroundView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void BackgroundView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QUrl url = event->mimeData()->urls().first();
        setImageUrl(url);
    }
}

void BackgroundView::hideEvent(QHideEvent *event)
{
    /* Disable hand scrolling when the view gets hidden in any way */
    setHandScrolling(false);
    QGraphicsView::hideEvent(event);
}

void BackgroundView::mousePressEvent(QMouseEvent *event)
{
    /* Activates hand scrolling when the middle mouse button is pressed. */
    if (event->button() == Qt::MidButton && isActiveWindow()) {
        setHandScrolling(true);
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void BackgroundView::mouseMoveEvent(QMouseEvent *event)
{
    /* Moves the view with the mouse while hand scrolling. */
    if (m_handScrolling) {
        auto *hBar = static_cast<FlexibleScrollBar*>(horizontalScrollBar());
        auto *vBar = static_cast<FlexibleScrollBar*>(verticalScrollBar());

        const QPoint d = event->globalPos() - m_lastMousePos;

        int horizontalValue = hBar->value() + (isRightToLeft() ? d.x() : -d.x());
        int verticalValue = vBar->value() - d.y();

        /* Panning can freely move the map without restriction on boundaries */
        hBar->forceSetValue(horizontalValue);
        vBar->forceSetValue(verticalValue);

        m_lastMousePos = event->globalPos();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
    m_lastMousePos = event->globalPos();
    m_lastMouseScenePos = mapToScene(viewport()->mapFromGlobal(m_lastMousePos));
}


void BackgroundView::mouseReleaseEvent(QMouseEvent *event)
{
    /* Deactivates hand scrolling when the middle mouse button is released. */
    if (event->button() == Qt::MidButton) {
        setHandScrolling(false);
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}


#ifndef QT_NO_WHEELEVENT
void BackgroundView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical) {

        QPointF target_viewport_pos = event->pos();
        QPointF target_scene_pos = QGraphicsView::mapToScene(event->pos());

        // The default zoom factor base is 1.0015.
        double  _zoom_factor_base = 1.0015;
        double angle = event->angleDelta().y();
        double factor = qPow(_zoom_factor_base, angle);

        /* gentle zoom */
        QGraphicsView::scale(factor, factor);
        QGraphicsView::centerOn(target_scene_pos);
        QPointF delta_viewport_pos =
                target_viewport_pos - QPointF(QGraphicsView::viewport()->width() / 2.0,
                                              QGraphicsView::viewport()->height() / 2.0);
        QPointF viewport_center = QGraphicsView::mapFromScene(target_scene_pos) - delta_viewport_pos;
        QGraphicsView::centerOn(QGraphicsView::mapToScene(viewport_center.toPoint()));
    }
}
#endif


