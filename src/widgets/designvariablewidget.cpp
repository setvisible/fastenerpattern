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

#include "designvariablewidget.h"
#include "ui_designvariablewidget.h"

#include <Core/AbstractSpliceModel>
#include <Core/Fastener>

#include <QtCore/QTimer>
#include <QtWidgets/QTableWidget>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

#define C_COLUMN_COUNT 3

DesignVariableWidget::DesignVariableWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::DesignVariableWidget)
  , m_tableTimer(new QTimer(this))
  , m_selectionTimer(new QTimer(this))
{
    ui->setupUi(this);

    QObject::connect(m_tableTimer, SIGNAL(timeout()), this, SLOT(updateTable()));
    QObject::connect(m_selectionTimer, SIGNAL(timeout()), this, SLOT(updateSelection()));

    QStringList labels;
    labels << tr("Name") <<  tr("Position X") << tr("Position Y");
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

DesignVariableWidget::~DesignVariableWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
void DesignVariableWidget::resizeColumnToContents()
{
    for(int i = 0; i < C_COLUMN_COUNT; i++)
        ui->tableWidget->resizeColumnToContents(i);
}

void DesignVariableWidget::onItemSelectionChanged()
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
void DesignVariableWidget::onFastenerInserted(const int, const Fastener &)
{
    updateTableLater(C_SHORT_DELAY_MSEC);
}

void DesignVariableWidget::onFastenerChanged(const int, const Fastener &)
{
    updateTableLater(C_LONG_DELAY_MSEC);
}

void DesignVariableWidget::onFastenerRemoved(const int)
{
    updateTableLater(C_SHORT_DELAY_MSEC);
}


void DesignVariableWidget::onSelectionFastenerChanged()
{
    updateSelectionLater(C_SHORT_DELAY_MSEC);
}

/******************************************************************************
 ******************************************************************************/
void DesignVariableWidget::updateTableLater(int msec)
{
    m_tableTimer->stop();
    m_tableTimer->start(msec);
}

void DesignVariableWidget::updateTable()
{
    m_tableTimer->stop();

    const int count = model()->fastenerCount();
    ui->tableWidget->setRowCount(count);

    Q_ASSERT( ui->tableWidget->rowCount() == count );
    for (int row = 0; row < count; ++row) {

        const Fastener ft = model()->fastenerAt(row);
        const double x = ft.positionX.value() *1000; // mm !
        const double y = ft.positionY.value() *1000; // mm !

        QTableWidgetItem *item_0 = new QTableWidgetItem( ft.name );
        QTableWidgetItem *item_1 = new QTableWidgetItem( QString("%0").arg( x, 0, 'f', 1) );
        QTableWidgetItem *item_2 = new QTableWidgetItem( QString("%0").arg( y, 0, 'f', 1) );

        ui->tableWidget->setItem(row, 0, item_0);
        ui->tableWidget->setItem(row, 1, item_1);
        ui->tableWidget->setItem(row, 2, item_2);
    }

    this->resizeColumnToContents();
}

/******************************************************************************
 ******************************************************************************/
void DesignVariableWidget::updateSelectionLater(int msec)
{
    m_selectionTimer->stop();
    m_selectionTimer->start(msec);
}

void DesignVariableWidget::updateSelection()
{
    while (m_tableTimer->isActive()) {
        /* Ensure the table is updated before updating the selection. */
        updateTable();
    }
    Q_ASSERT(!m_tableTimer->isActive());
    m_selectionTimer->stop();

    bool blocked1 = ui->tableWidget->blockSignals(true);
    bool blocked2 = m_selectionTimer->blockSignals(true);
    bool blocked3 = m_tableTimer->blockSignals(true);

    QSet<int> set = model()->selectedFastenerIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        bool selected = set.contains(row);
        for (int col = 0; col < C_COLUMN_COUNT; ++col) {
            ui->tableWidget->item(row, col)->setSelected( selected );
        }
    }

    ui->tableWidget->blockSignals(blocked1);
    m_selectionTimer->blockSignals(blocked2);
    m_tableTimer->blockSignals(blocked3);
}
