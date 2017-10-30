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

#include "designspacewidget.h"
#include "ui_designspacewidget.h"

#include <QtCore/QDebug>
#include <QtWidgets/QTableWidget>

#define C_COLUMN_COUNT 1

DesignSpaceWidget::DesignSpaceWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::DesignSpaceWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("Name");
    ui->tableWidget->setColumnCount( C_COLUMN_COUNT );
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
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
    QObject::connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
                     this, SLOT(onItemDataChanged(QTableWidgetItem*)));

}

DesignSpaceWidget::~DesignSpaceWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
void DesignSpaceWidget::onItemDataChanged(QTableWidgetItem *item)
{
    const int index = item->row();
    DesignSpace ds = model()->designSpaceAt(index);
    ds.name = item->text();
    model()->setDesignSpace(index, ds);
}

void DesignSpaceWidget::onItemSelectionChanged()
{
    QSet<int> set;
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item && item->isSelected()) {
            set << row;
        }
    }
    model()->setDesignSpaceSelection(set);
}

/******************************************************************************
 ******************************************************************************/
void DesignSpaceWidget::onDesignSpaceInserted(const int index, const DesignSpace &designSpace)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(designSpace.name);
    ui->tableWidget->insertRow(index);
    ui->tableWidget->setItem(index, 0, newItem);
}

void DesignSpaceWidget::onDesignSpaceChanged(const int index, const DesignSpace &designSpace)
{
    QTableWidgetItem *item = ui->tableWidget->item(index, 0);
    if (item)
        item->setText(designSpace.name);
}

void DesignSpaceWidget::onDesignSpaceRemoved(const int index)
{
    ui->tableWidget->removeRow(index);
}

void DesignSpaceWidget::onSelectionDesignSpaceChanged()
{
    const QSet<int> set = model()->selectedDesignSpaceIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        const bool selected = set.contains(row);
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item)
            item->setSelected( selected );
    }
}
