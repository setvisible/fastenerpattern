/* - FastenerPattern - Copyright (C) 2016-2017 Sebastien Vavassori
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

#include "splicegraphicswidget.h"

#include "backgroundwidget.h"
#include "items/appliedloaditem.h"
#include "items/designspaceitem.h"
#include "items/fasteneritem.h"
#include "items/measureitem.h"
#include "items/utils_scale.h"

#include <Maths/Delaunay>

#include <QtCore/QSet>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGraphicsScene>

/*! \class SpliceGraphicsWidget
 *  \brief The class SpliceGraphicsWidget is the main GUI Graphics View.
 *
 */

SpliceGraphicsWidget::SpliceGraphicsWidget(QWidget *parent) : AbstractSpliceView(parent)
  , m_mainLayout(new QVBoxLayout(this))
  , m_backgroundWidget(new BackgroundWidget(this))
  , m_componentVisible(false)
  , m_resultantVisible(true)
  , m_torqueVisible(true)
  , m_labelVisible(false)
  , m_snapEnable(false)
  , m_distanceVisible(false)
  , m_designSpaceVisible(true)
{
    m_backgroundWidget->setFlags(
                QFlags<BackgroundWidget::Feature>(
                    BackgroundWidget::HorizontalAxis
                    | BackgroundWidget::VerticalAxis
                    | BackgroundWidget::Grid
                    | BackgroundWidget::DragImageHereText), true);

    QGroupBox* gb = new QGroupBox(tr("Splice"));
    QVBoxLayout* gbLayout = new QVBoxLayout();
    gbLayout->addWidget(m_backgroundWidget);
    gb->setLayout(gbLayout);

    m_mainLayout->addWidget(gb);
    this->setLayout(m_mainLayout);

    QObject::connect(m_backgroundWidget->scene(), SIGNAL(selectionChanged()),
                     this, SLOT(onSelectionChanged()));

    /* Create immuable items. */
    m_appliedLoadItem = new AppliedLoadItem();
    m_backgroundWidget->scene()->addItem(m_appliedLoadItem);

    /// \todo Auto resize instead ?
    m_backgroundWidget->scene()->setSceneRect(-1000, -1000, 2000, 2000);

}

SpliceGraphicsWidget::~SpliceGraphicsWidget()
{
    // delete ui;
}

void SpliceGraphicsWidget::update()
{
    m_backgroundWidget->scene()->update();
}

/******************************************************************************
 ******************************************************************************/
void SpliceGraphicsWidget::onSelectionChanged()
{
    {
        /* fastener items */
        QSet<int> set;
        for (int i = 0; i < m_fastenerItems.count(); ++i) {
            FastenerItem* item = m_fastenerItems.at(i);
            if (item->isSelected()) {
                set << i;
            }
        }
        model()->setFastenerSelection(set);
    }
    {
        /* design space items */
        QSet<int> set;
        for (int i = 0; i < m_designSpaceItems.count(); ++i) {
            DesignSpaceItem* item = m_designSpaceItems.at(i);
            if (item->isSelected()) {
                set << i;
            }
        }
        model()->setDesignSpaceSelection(set);
    }
}

void SpliceGraphicsWidget::onFastenerPositionChanged()
{
    FastenerItem *item = static_cast<FastenerItem *>(sender());
    if (!item)
        return;

    int index = m_fastenerItems.indexOf(item);
    Fastener fastener = model()->fastenerAt(index);
    fastener.positionX = item->truePositionX() *_mm;
    fastener.positionY = item->truePositionY() *_mm;
    model()->setFastener(index, fastener);
}

/******************************************************************************
 ******************************************************************************/
void SpliceGraphicsWidget::onFastenerInserted(const int index, const Fastener &fastener)
{
    FastenerItem *item = new FastenerItem();
    QObject::connect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
    QObject::connect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));

    m_backgroundWidget->scene()->addItem(item);
    m_fastenerItems.insert(index, item);

    item->setTruePosition(fastener.positionX.value() *1000,
                          fastener.positionY.value() *1000);
    item->setTrueDiameter(fastener.diameter.value() *1000);

    item->setResultantVisible(m_resultantVisible);
    item->setComponentVisible(m_componentVisible);
    item->setTorqueVisible(m_torqueVisible);
    item->setLabelVisible(m_labelVisible);

    this->updateDistanceItemPositions();
}

void SpliceGraphicsWidget::onFastenerChanged(const int index, const Fastener &fastener)
{
    if (index >= 0 && index < m_fastenerItems.count()) {
        FastenerItem *item = m_fastenerItems[index];
        item->setTruePosition(fastener.positionX.value() *1000,
                              fastener.positionY.value() *1000);
        item->setTrueDiameter(fastener.diameter.value() *1000);
    }
    this->updateDistanceItemPositions();
}

void SpliceGraphicsWidget::onFastenerRemoved(const int index)
{
    if (index >= 0 && index < m_fastenerItems.count()) {
        FastenerItem* item = m_fastenerItems.takeAt(index);
        QObject::disconnect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
        QObject::disconnect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));
        m_backgroundWidget->scene()->removeItem(item);
    }
    this->updateDistanceItemPositions();
}

/******************************************************************************
 ******************************************************************************/
void SpliceGraphicsWidget::onDesignSpaceInserted(const int index, const DesignSpace &designSpace)
{
    DesignSpaceItem *item = new DesignSpaceItem();
    // QObject::connect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
    // QObject::connect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));

    m_backgroundWidget->scene()->addItem(item);
    m_designSpaceItems.insert(index, item);

    item->setPolygon( designSpace.polygon );
}

void SpliceGraphicsWidget::onDesignSpaceChanged(const int index, const DesignSpace &designSpace)
{

}

void SpliceGraphicsWidget::onDesignSpaceRemoved(const int index)
{
    if (index >= 0 && index < m_designSpaceItems.count()) {
        DesignSpaceItem* item = m_designSpaceItems.takeAt(index);
        // QObject::disconnect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
        // QObject::disconnect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));
        m_backgroundWidget->scene()->removeItem(item);

    }

}

/******************************************************************************
 ******************************************************************************/
void SpliceGraphicsWidget::onAppliedLoadChanged()
{
    m_appliedLoadItem->setTensor( model()->appliedLoad() );
}

void SpliceGraphicsWidget::onResultsChanged()
{
    const int count = model()->fastenerCount();
    if (count != m_fastenerItems.count()) {
        return;
    }
    for (int index = 0; index < count; ++index) {
        FastenerItem *item = m_fastenerItems[index];
        item->setResult(model()->resultAt(index));
    }
}

/******************************************************************************
 ******************************************************************************/
void SpliceGraphicsWidget::onSelectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
    for( int i = 0; i < m_fastenerItems.count(); ++i) {
        m_fastenerItems.at(i)->setSelected(set.contains(i));
    }
}

void SpliceGraphicsWidget::onSelectionDesignSpaceChanged()
{
    QSet<int> set = model()->selectedDesignSpaceIndexes();
    for( int i = 0; i < m_designSpaceItems.count(); ++i) {
        m_designSpaceItems.at(i)->setSelected(set.contains(i));
    }
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isAxesVisible() const
{
    return m_backgroundWidget->testFlag(BackgroundWidget::HorizontalAxis)
            && m_backgroundWidget->testFlag(BackgroundWidget::VerticalAxis);
}

void SpliceGraphicsWidget::setAxesVisible(bool visible)
{
    QFlags<BackgroundWidget::Feature> flags
            = BackgroundWidget::HorizontalAxis
            | BackgroundWidget::VerticalAxis;
    m_backgroundWidget->setFlags(flags, visible);
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isGridVisible() const
{
    return m_backgroundWidget->testFlag(BackgroundWidget::Grid);
}

void SpliceGraphicsWidget::setGridVisible(bool visible)
{
    m_backgroundWidget->setFlag(BackgroundWidget::Grid, visible);
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isImageVisible() const
{
    return m_backgroundWidget->testFlag(BackgroundWidget::Image);
}

void SpliceGraphicsWidget::setImageVisible(bool visible)
{
    m_backgroundWidget->setFlag(BackgroundWidget::Image, visible);
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isComponentVisible() const
{
    return m_componentVisible;
}

void SpliceGraphicsWidget::setComponentVisible(bool visible)
{
    m_componentVisible = visible;
    m_appliedLoadItem->setComponentVisible(visible);
    foreach (auto &f, m_fastenerItems) {
        f->setComponentVisible(visible);
    }
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isResultantVisible() const
{
    return m_resultantVisible;
}

void SpliceGraphicsWidget::setResultantVisible(bool visible)
{
    m_resultantVisible = visible;
    m_appliedLoadItem->setResultantVisible(visible);
    foreach (auto &f, m_fastenerItems) {
        f->setResultantVisible(visible);
    }
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isTorqueVisible() const
{
    return m_torqueVisible;
}

void SpliceGraphicsWidget::setTorqueVisible(bool visible)
{
    m_torqueVisible = visible;
    m_appliedLoadItem->setTorqueVisible(visible);
    foreach (auto &f, m_fastenerItems) {
        f->setTorqueVisible(visible);
    }
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isLabelVisible() const
{
    return m_labelVisible;
}

void SpliceGraphicsWidget::setLabelVisible(bool visible)
{
    m_labelVisible = visible;
    m_appliedLoadItem->setLabelVisible(visible);
    foreach (auto &f, m_fastenerItems) {
        f->setLabelVisible(visible);
    }
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isSnapEnable() const
{
    return m_snapEnable;
}

void SpliceGraphicsWidget::setSnapEnable(bool enable)
{
    /// \todo implement setSnapEnable
    m_snapEnable = enable;
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isDistanceVisible() const
{
    return m_distanceVisible;
}

void SpliceGraphicsWidget::setDistanceVisible(bool visible)
{
    m_distanceVisible = visible;
    this->updateDistanceItemPositions();
}

void SpliceGraphicsWidget::updateDistanceItemPositions()
{
    while (!m_measureItems.isEmpty()) {
        MeasureItem *item = m_measureItems.takeLast();
        m_backgroundWidget->scene()->removeItem(item);
        delete item;
    }
    if (m_distanceVisible) {
        QList<QPointF> points;
        foreach (auto &f, m_fastenerItems) {
            points << f->pos();
        }
        const QList<QLineF> lines = Maths::delaunayTriangulation( points );
        foreach (auto &line, lines) {
            MeasureItem *item = new MeasureItem();
            qreal trueLength = line.length() / C_DEFAULT_SCREEN_DPI;
            item->setLine(line);
            item->setEndSpace( 2.5 );
            item->setColor( QColor(136, 0, 21) ); /* Brown */
            item->setText(QString("%0mm").arg(trueLength, 0, 'f', 1));
            m_backgroundWidget->scene()->addItem(item);
            m_measureItems << item;
        }
    }
}

/******************************************************************************
 ******************************************************************************/
qreal SpliceGraphicsWidget::pixelsPerUnit() const
{
    return m_backgroundWidget->pixelsPerUnit();
}

void SpliceGraphicsWidget::setPixelsPerUnit(qreal pxPerUnit)
{
    m_backgroundWidget->setPixelsPerUnit(pxPerUnit);
}

/******************************************************************************
 ******************************************************************************/
QUrl SpliceGraphicsWidget::imageUrl() const
{
    return m_backgroundWidget->imageUrl();
}

void SpliceGraphicsWidget::setImageUrl(const QUrl &url)
{
    m_backgroundWidget->setImageUrl(url);
}

/******************************************************************************
 ******************************************************************************/
bool SpliceGraphicsWidget::isDesignSpaceVisible() const
{
    return m_designSpaceVisible;
}

void SpliceGraphicsWidget::setDesignSpaceVisible(bool visible)
{
    foreach (auto &item, m_designSpaceItems) {
        if (!visible) {
            item->setSelected(false);
        }
        item->setVisible(visible);
    }
    m_designSpaceVisible = visible;
}
