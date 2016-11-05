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

#include <QtCore/QDebug>

TableWidget::TableWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::TableWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("X") << tr("Y") << tr("d") << tr("t") << tr("dof X") << tr("dof Y");
    ui->treeWidget->setColumnCount( 6 );
    ui->treeWidget->setHeaderLabels(labels);

    QObject::connect(ui->treeWidget, SIGNAL(itemSelectionChanged()),
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
        ui->treeWidget->resizeColumnToContents(i);
}

/***********************************************************************************
 ***********************************************************************************/
void TableWidget::fastenersInserted(const int index, const Fastener &fastener)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    ui->treeWidget->insertTopLevelItem(index, item);
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);

    item->setText(0, QString("%0").arg( fastener.positionX.value(), 0, 'f', 3));
    item->setText(1, QString("%0").arg( fastener.positionY.value(), 0, 'f', 3));
    item->setText(2, QString("%0").arg( fastener.diameter.value() , 0, 'f', 3));
    item->setText(3, QString("%0").arg( fastener.thickness.value(), 0, 'f', 3));
    item->setText(4, (fastener.DoF_X ? QStringLiteral("fixed") : QStringLiteral("free")));
    item->setText(5, (fastener.DoF_Y ? QStringLiteral("fixed") : QStringLiteral("free")));

    this->resizeColumnToContents();
}

void TableWidget::fastenersChanged(const int index, const Fastener &fastener)
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(index);
    if (!item)
        return;

    item->setText(0, QString("%0").arg( fastener.positionX.value(), 0, 'f', 3));
    item->setText(1, QString("%0").arg( fastener.positionY.value(), 0, 'f', 3));
    item->setText(2, QString("%0").arg( fastener.diameter.value() , 0, 'f', 3));
    item->setText(3, QString("%0").arg( fastener.thickness.value(), 0, 'f', 3));
    item->setText(4, (fastener.DoF_X ? QStringLiteral("fixed") : QStringLiteral("free")));
    item->setText(5, (fastener.DoF_Y ? QStringLiteral("fixed") : QStringLiteral("free")));

    this->resizeColumnToContents();
}

void TableWidget::fastenersRemoved(const int index)
{
    QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(index);
    if (item)
        delete item;
    this->resizeColumnToContents();

}

/***********************************************************************************
 ***********************************************************************************/

void TableWidget::onItemSelectionChanged()
{
    QSet<int> set;
    QTreeWidgetItem *rootItem = ui->treeWidget->invisibleRootItem();

    if (!rootItem)
        return;

    int i = rootItem->childCount();
    while (i>0) {
        i--;
        QTreeWidgetItem *item = rootItem->child(i);
        if (item->isSelected())
            set << i;
    }
    model()->setSelection(set);
}


void TableWidget::selectionChanged()
{
    QSet<int> set = model()->selectedIndexes();
    QTreeWidgetItem *rootItem = ui->treeWidget->invisibleRootItem();

    if (!rootItem)
        return;

    int i = rootItem->childCount();
    while (i>0) {
        i--;
        QTreeWidgetItem *item = rootItem->child(i);
        if (item)
            ui->treeWidget->setItemSelected(item, set.contains(i));
    }
}

