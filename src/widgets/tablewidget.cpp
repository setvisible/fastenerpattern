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

void TableWidget::onFastenerInserted(const int index, const Fastener &fastener)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    ui->treeWidget->insertTopLevelItem(index, item);
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);

    item->setText(0, QString("%0").arg( 1000 * fastener.positionX.value(), 0, 'f', 1));
    item->setText(1, QString("%0").arg( 1000 * fastener.positionY.value(), 0, 'f', 1));
    item->setText(2, QString("%0").arg( 1000 * fastener.diameter.value() , 0, 'f', 2));
    item->setText(3, QString("%0").arg( 1000 * fastener.thickness.value(), 0, 'f', 2));
    item->setText(4, DOFToString(fastener.DoF_X));
    item->setText(5, DOFToString(fastener.DoF_Y));

    this->resizeColumnToContents();
}

void TableWidget::onFastenerChanged(const int index, const Fastener &fastener)
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(index);
    if (!item)
        return;

    item->setText(0, QString("%0").arg( 1000 * fastener.positionX.value(), 0, 'f', 1));
    item->setText(1, QString("%0").arg( 1000 * fastener.positionY.value(), 0, 'f', 1));
    item->setText(2, QString("%0").arg( 1000 * fastener.diameter.value() , 0, 'f', 2));
    item->setText(3, QString("%0").arg( 1000 * fastener.thickness.value(), 0, 'f', 2));
    item->setText(4, DOFToString(fastener.DoF_X));
    item->setText(5, DOFToString(fastener.DoF_Y));

    this->resizeColumnToContents();
}

void TableWidget::onFastenerRemoved(const int index)
{
    QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(index);
    if (item)
        delete item;
    this->resizeColumnToContents();

}

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
    model()->setFastenerSelection(set);
}


void TableWidget::onSelectionFastenerChanged()
{
    QSet<int> set = model()->selectedFastenerIndexes();
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
