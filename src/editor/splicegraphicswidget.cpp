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

#include "splicegraphicswidget.h"

#include "backgroundwidget.h"
#include "items/appliedloaditem.h"
#include "items/fasteneritem.h"

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
{
    m_backgroundWidget->enableFeatures(
                QFlags<BackgroundWidget::Features>(
                    BackgroundWidget::HorizontalAxis
                    | BackgroundWidget::VerticalAxis
                    | BackgroundWidget::DragImageHereText));

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

    /// TODO : Auto resize instead ?
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

/***********************************************************************************
 ***********************************************************************************/
void SpliceGraphicsWidget::onSelectionChanged()
{
    QSet<int> set;
    for (int i = 0; i < m_fastenerItems.count(); ++i) {
        FastenerItem* item = m_fastenerItems.at(i);
        if (item->isSelected()) {
            set << i;
        }
    }
    model()->setSelection(set);
}

/***********************************************************************************
 ***********************************************************************************/
void SpliceGraphicsWidget::onFastenerPositionChanged()
{
    FastenerItem *item = static_cast<FastenerItem *>(sender());
    if (!item)
        return;

    int index = m_fastenerItems.indexOf(item);
    Fastener fastener = model()->fastenerAt(index);
    fastener.positionX = item->truePositionX() *mm;
    fastener.positionY = item->truePositionY() *mm;
    model()->setFastener(index, fastener);
  }

void SpliceGraphicsWidget::fastenersInserted(const int index, const Fastener &fastener)
{
    FastenerItem *item = new FastenerItem();
    QObject::connect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
    QObject::connect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));

    m_backgroundWidget->scene()->addItem(item);
    m_fastenerItems.insert(index, item);

    item->setTruePosition(fastener.positionX.value() *1000, fastener.positionY.value() *1000);
    item->setTrueDiameter(fastener.diameter.value() *1000);
}

void SpliceGraphicsWidget::fastenersChanged(const int index, const Fastener &fastener)
{
    if (index >= 0 && index < m_fastenerItems.count()) {
        FastenerItem *item = m_fastenerItems[index];
        item->setTruePosition(fastener.positionX.value() *1000, fastener.positionY.value() *1000);
        item->setTrueDiameter(fastener.diameter.value() *1000);
    }
}

void SpliceGraphicsWidget::fastenersRemoved(const int index)
{
    if (index >= 0 && index < m_fastenerItems.count()) {
        FastenerItem* item = m_fastenerItems.takeAt(index);
        QObject::disconnect(item, SIGNAL(xChanged()), this, SLOT(onFastenerPositionChanged()));
        QObject::disconnect(item, SIGNAL(yChanged()), this, SLOT(onFastenerPositionChanged()));
        m_backgroundWidget->scene()->removeItem(item);
    }
}

void SpliceGraphicsWidget::appliedLoadChanged()
{
    m_appliedLoadItem->setTensor( model()->loadcase() );
}

void SpliceGraphicsWidget::selectionChanged()
{
    QSet<int> set = model()->selectedIndexes();
    for( int i = 0; i < m_fastenerItems.count(); ++i) {
        m_fastenerItems.at(i)->setSelected(set.contains(i));
    }
}

void SpliceGraphicsWidget::resultsChanged()
{
    const int count = model()->fastenerCount();
    if (count != m_fastenerItems.count())
        return;
    for (int index = 0; index < count; ++index) {
        FastenerItem *item = m_fastenerItems[index];
        item->setResult(model()->resultAt(index));
    }
}

bool SpliceGraphicsWidget::isAxesVisible() const
{
    return m_backgroundWidget->isAxesVisible();
}

void SpliceGraphicsWidget::setAxesVisible(bool visible)
{
    m_backgroundWidget->setAxesVisible(visible);
}

bool SpliceGraphicsWidget::isGridVisible() const
{
    return m_backgroundWidget->isGridVisible();
}

void SpliceGraphicsWidget::setGridVisible(bool visible)
{
    m_backgroundWidget->setGridVisible(visible);
}

bool SpliceGraphicsWidget::isImageVisible() const
{
    return m_backgroundWidget->isImageVisible();
}

void SpliceGraphicsWidget::setImageVisible(bool visible)
{
    m_backgroundWidget->setImageVisible(visible);
}

qreal SpliceGraphicsWidget::pixelsPerUnit() const
{
    return m_backgroundWidget->pixelsPerUnit();
}

void SpliceGraphicsWidget::setPixelsPerUnit(qreal pxPerUnit)
{
    m_backgroundWidget->setPixelsPerUnit(pxPerUnit);
}

QUrl SpliceGraphicsWidget::imageUrl() const
{
    return m_backgroundWidget->imageUrl();
}

void SpliceGraphicsWidget::setImageUrl(const QUrl &url)
{
    m_backgroundWidget->setImageUrl(url);
}
