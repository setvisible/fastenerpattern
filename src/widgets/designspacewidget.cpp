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

#include <boost/units/cmath.hpp> /* pow<>() */

#define C_COLUMN_COUNT 4
#define C_SECTION_SIZE 20

DesignSpaceWidget::DesignSpaceWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::DesignSpaceWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("Name") <<  tr("FX") << tr("FY") << tr("Resultant");
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

DesignSpaceWidget::~DesignSpaceWidget()
{
    delete ui;
}

void DesignSpaceWidget::resizeColumnToContents()
{
    for(int i = 0; i < C_COLUMN_COUNT; i++)
        ui->tableWidget->resizeColumnToContents(i);
}

void DesignSpaceWidget::onItemSelectionChanged()
{
    QSet<int> set;
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        if ( ui->tableWidget->item(row,0)->isSelected() )
            set << row;
    }
    model()->setSelection(set);
}

void DesignSpaceWidget::selectionChanged()
{
    QSet<int> set = model()->selectedIndexes();
    int row = ui->tableWidget->rowCount();
    while (row>0) {
        row--;
        bool selected = set.contains(row);
        for (int col = 0; col < C_COLUMN_COUNT; ++col) {
            ui->tableWidget->item(row, col)->setSelected( selected );
        }
    }
}

void DesignSpaceWidget::resultsChanged()
{
    const int count = model()->fastenerCount();
    ui->tableWidget->setRowCount(count);

    Q_ASSERT( ui->tableWidget->rowCount() == count );
    for (int row = 0; row < count; ++row) {
        Tensor res = model()->resultAt(row);
        Fastener ft = model()->fastenerAt(row);

        Force resultant =
                boost::units::root<2>(
                    boost::units::pow<2>(res.force_x)
                    + boost::units::pow<2>(res.force_y));

        QTableWidgetItem *item_0 = new QTableWidgetItem( ft.name );
        QTableWidgetItem *item_1 = new QTableWidgetItem( QString("%0").arg( res.force_x.value(), 0, 'f', 1) );
        QTableWidgetItem *item_2 = new QTableWidgetItem( QString("%0").arg( res.force_y.value(), 0, 'f', 1) );
        QTableWidgetItem *item_3 = new QTableWidgetItem( QString("%0").arg( resultant.value(), 0, 'f', 1) );

        ui->tableWidget->setItem(row, 0, item_0);
        ui->tableWidget->setItem(row, 1, item_1);
        ui->tableWidget->setItem(row, 2, item_2);
        ui->tableWidget->setItem(row, 3, item_3);
    }

    this->resizeColumnToContents();
}
