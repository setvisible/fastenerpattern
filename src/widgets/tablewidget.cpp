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

#include "tablewidget.h"
#include "ui_tablewidget.h"

#include <Core/AbstractSpliceModel>
#include <Core/Fastener>

#include <QtCore/QDebug>

static inline QString DOFToString(Fastener::DOF dof)
{
    return (dof == Fastener::Fixed) ? QObject::tr("fixed") : QObject::tr("free");
}


TableWidget::TableWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::TableWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("X") << tr("Y") << tr("d") << tr("t") << tr("dof X") << tr("dof Y");
    ui->tableWidget->setColumnCount( 6 );
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

TableWidget::~TableWidget()
{
    delete ui;
}

void TableWidget::resizeColumnToContents()
{
    for(int i = 0; i < 6; i++)
        ui->tableWidget->resizeColumnToContents(i);
}

void TableWidget::onFastenerInserted(const int index, const Fastener &fastener)
{
    Q_UNUSED(index)
    Q_UNUSED(fastener)
    updateTable();
}

void TableWidget::onFastenerChanged(const int index, const Fastener &fastener)
{
    Q_UNUSED(index)
    Q_UNUSED(fastener)
    updateTable();
}

void TableWidget::onFastenerRemoved(const int index)
{
    Q_UNUSED(index)
    updateTable();
}

void TableWidget::onItemSelectionChanged()
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

void TableWidget::onSelectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        bool selected = set.contains(row);
        for (int col = 0; col < 6; ++col) {
            ui->tableWidget->item(row, col)->setSelected( selected );
        }
    }
}

void TableWidget::updateTable()
{
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
