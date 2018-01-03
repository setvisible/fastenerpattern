/* - FastenerPattern - Copyright (C) 2016-2018 Sebastien Vavassori
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

#include "fastenertablewidget.h"
#include "ui_fastenertablewidget.h"

#include <Core/AbstractSpliceModel>
#include <Core/Fastener>

#include <QtCore/QTimer>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

#define C_COLUMN_COUNT 6

static inline QString DOFToString(Fastener::DOF dof)
{
    return (dof == Fastener::Fixed) ? QObject::tr("fixed") : QObject::tr("free");
}

FastenerTableWidget::FastenerTableWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::FastenerTableWidget)
  , m_tableTimer(new QTimer(this))
  , m_selectionTimer(new QTimer(this))
{
    ui->setupUi(this);

    QObject::connect(m_tableTimer, SIGNAL(timeout()), this, SLOT(updateTable()));
    QObject::connect(m_selectionTimer, SIGNAL(timeout()), this, SLOT(updateSelection()));

    QStringList labels;
    labels << tr("X") << tr("Y") << tr("d") << tr("t") << tr("dof X") << tr("dof Y");
    ui->tableWidget->setColumnCount( C_COLUMN_COUNT );
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    int size = ui->tableWidget->verticalHeader()->minimumSectionSize();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(size);

    ui->tableWidget->setAlternatingRowColors(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QObject::connect(ui->tableWidget, SIGNAL(itemSelectionChanged()),
                     this, SLOT(onItemSelectionChanged()));

    this->resizeColumnToContents();
}

FastenerTableWidget::~FastenerTableWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
void FastenerTableWidget::resizeColumnToContents()
{
    for(int i = 0; i < C_COLUMN_COUNT; i++)
        ui->tableWidget->resizeColumnToContents(i);
}

void FastenerTableWidget::onItemSelectionChanged()
{
    QSet<int> set;
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        if ( ui->tableWidget->item(row,0)->isSelected() )
            set << row;
    }
    model()->setFastenerSelection(set);
}

/******************************************************************************
 ******************************************************************************/
void FastenerTableWidget::onFastenerInserted(const int, const Fastener &)
{
    updateTableLater(C_SHORT_DELAY_MSEC);
}

void FastenerTableWidget::onFastenerChanged(const int, const Fastener &)
{
    updateTableLater(C_LONG_DELAY_MSEC);
}

void FastenerTableWidget::onFastenerRemoved(const int)
{
    updateTableLater(C_SHORT_DELAY_MSEC);
}

void FastenerTableWidget::onSelectionFastenerChanged()
{
    updateSelectionLater(C_SHORT_DELAY_MSEC);
}

/******************************************************************************
 ******************************************************************************/
void FastenerTableWidget::updateTableLater(int msec)
{
    m_tableTimer->stop();
    m_tableTimer->start(msec);
}

void FastenerTableWidget::updateTable()
{
    m_tableTimer->stop();

    const int count = model()->fastenerCount();
    ui->tableWidget->setRowCount(count);

    Q_ASSERT( ui->tableWidget->rowCount() == count );
    for (int row = 0; row < count; ++row) {

        const Fastener ft = model()->fastenerAt(row);
        const double x = ft.positionX.value() *1000; // mm !
        const double y = ft.positionY.value() *1000; // mm !
        const double d = ft.diameter.value()  *1000; // mm !
        const double t = ft.thickness.value() *1000; // mm !

        QTableWidgetItem *item_0 = new QTableWidgetItem( ft.name );
        QTableWidgetItem *item_1 = new QTableWidgetItem( QString("%0").arg( x, 0, 'f', 1) );
        QTableWidgetItem *item_2 = new QTableWidgetItem( QString("%0").arg( y, 0, 'f', 1) );
        QTableWidgetItem *item_3 = new QTableWidgetItem( QString("%0").arg( d, 0, 'f', 1) );
        QTableWidgetItem *item_4 = new QTableWidgetItem( QString("%0").arg( t, 0, 'f', 1) );
        QTableWidgetItem *item_5 = new QTableWidgetItem( DOFToString( ft.DoF_X ) );
        QTableWidgetItem *item_6 = new QTableWidgetItem( DOFToString( ft.DoF_Y ) );

        ui->tableWidget->setItem(row, 0, item_0);
        ui->tableWidget->setItem(row, 1, item_1);
        ui->tableWidget->setItem(row, 2, item_2);
        ui->tableWidget->setItem(row, 3, item_3);
        ui->tableWidget->setItem(row, 4, item_4);
        ui->tableWidget->setItem(row, 5, item_5);
        ui->tableWidget->setItem(row, 6, item_6);
    }

    this->resizeColumnToContents();
}

/******************************************************************************
 ******************************************************************************/
void FastenerTableWidget::updateSelectionLater(int msec)
{
    m_selectionTimer->stop();
    m_selectionTimer->start(msec);
}

void FastenerTableWidget::updateSelection()
{
    while (m_tableTimer->isActive()) {
        /* Ensure the table is updated before updating the selection. */
        updateTable();
    }
    Q_ASSERT(!m_tableTimer->isActive());
    m_selectionTimer->stop();

    bool blocked = ui->tableWidget->blockSignals(true);
    QSet<int> set = model()->selectedFastenerIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        bool selected = set.contains(row);
        for (int col = 0; col < C_COLUMN_COUNT; ++col) {
            ui->tableWidget->item(row, col)->setSelected( selected );
        }
    }
    ui->tableWidget->blockSignals(blocked);
}
