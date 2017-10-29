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

#include <QtCore/QDebug>
#include <QtWidgets/QTableWidget>

#define C_COLUMN_COUNT 3
#define C_SECTION_SIZE 20

DesignVariableWidget::DesignVariableWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::DesignVariableWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("Name") <<  tr("Position X") << tr("Position Y");
    ui->tableWidget->setColumnCount( C_COLUMN_COUNT );
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize( C_SECTION_SIZE );
    ui->tableWidget->setAlternatingRowColors(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QObject::connect(ui->tableWidget, SIGNAL(itemSelectionChanged()),
                     this, SLOT(onItemSelectionChanged()));

    this->resizeColumnToContents();
}

DesignVariableWidget::~DesignVariableWidget()
{
    delete ui;
}

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


void DesignVariableWidget::fastenersInserted(const int index, const Fastener &fastener)
{
    Q_UNUSED(index)
    Q_UNUSED(fastener)
    updateTable();
}

void DesignVariableWidget::fastenersChanged(const int index, const Fastener &fastener)
{
    Q_UNUSED(index)
    Q_UNUSED(fastener)
    updateTable();
}

void DesignVariableWidget::fastenersRemoved(const int index)
{
    Q_UNUSED(index)
    updateTable();
}


void DesignVariableWidget::selectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        bool selected = set.contains(row);
        for (int col = 0; col < C_COLUMN_COUNT; ++col) {
            ui->tableWidget->item(row, col)->setSelected( selected );
        }
    }
}

void DesignVariableWidget::updateTable()
{
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
