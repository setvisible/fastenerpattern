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

#include "optimisationwidget.h"
#include "ui_optimisationwidget.h"

#include <boost/units/cmath.hpp> /* pow<>() */

#include <QtCore/QDebug>

OptimisationWidget::OptimisationWidget(QWidget *parent) : AbstractSpliceView(parent)
  , ui(new Ui::OptimisationWidget)
{
    ui->setupUi(this);

    ui->treeWidget->setExpandsOnDoubleClick(false);
    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->expandAll();

    /*
    QObject::connect(ui->treeWidget, SIGNAL(itemSelectionChanged()),
                     this, SLOT(onItemSelectionChanged()));
*/

}

OptimisationWidget::~OptimisationWidget()
{
    delete ui;
}

void OptimisationWidget::fastenersInserted(const int index, const Fastener &fastener)
{
  //  qDebug() << Q_FUNC_INFO;
    /*  QTreeWidgetItem* item = new QTreeWidgetItem();
    ui->treeWidget->insertTopLevelItem(index, item);
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);

    item->setText(0, QString("%0").arg( 1000 * fastener.positionX.value(), 0, 'f', 1));
    item->setText(1, QString("%0").arg( 1000 * fastener.positionY.value(), 0, 'f', 1));
    item->setText(2, QString("%0").arg( 1000 * fastener.diameter.value() , 0, 'f', 2));
    item->setText(3, QString("%0").arg( 1000 * fastener.thickness.value(), 0, 'f', 2));
    item->setText(4, DOFToString(fastener.DoF_X));
    item->setText(5, DOFToString(fastener.DoF_Y));

    this->resizeColumnToContents();*/
}

void OptimisationWidget::fastenersChanged(const int index, const Fastener &fastener)
{
   // qDebug() << Q_FUNC_INFO;
    /*  QTreeWidgetItem* item = ui->treeWidget->topLevelItem(index);
    if (!item)
        return;

    item->setText(0, QString("%0").arg( 1000 * fastener.positionX.value(), 0, 'f', 1));
    item->setText(1, QString("%0").arg( 1000 * fastener.positionY.value(), 0, 'f', 1));
    item->setText(2, QString("%0").arg( 1000 * fastener.diameter.value() , 0, 'f', 2));
    item->setText(3, QString("%0").arg( 1000 * fastener.thickness.value(), 0, 'f', 2));
    item->setText(4, DOFToString(fastener.DoF_X));
    item->setText(5, DOFToString(fastener.DoF_Y));

    this->resizeColumnToContents();*/
}

void OptimisationWidget::fastenersRemoved(const int index)
{
  //  qDebug() << Q_FUNC_INFO;
    /* QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(index);
    if (item)
        delete item;
    this->resizeColumnToContents();*/

}

void OptimisationWidget::onItemSelectionChanged()
{
  //  qDebug() << Q_FUNC_INFO;
    /*  QSet<int> set;
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
    model()->setSelection(set);*/
}


void OptimisationWidget::selectionFastenerChanged()
{
  //  qDebug() << Q_FUNC_INFO;
    /* QSet<int> set = model()->selectedIndexes();
    QTreeWidgetItem *rootItem = ui->treeWidget->invisibleRootItem();

    if (!rootItem)
        return;

    int i = rootItem->childCount();
    while (i>0) {
        i--;
        QTreeWidgetItem *item = rootItem->child(i);
        if (item)
            ui->treeWidget->setItemSelected(item, set.contains(i));
    }*/
}

void OptimisationWidget::resultsChanged()
{
 //   qDebug() << Q_FUNC_INFO;

    Force maxAbs = 0.0*N;
    const int count = model()->fastenerCount();
    for (int index = 0; index < count; ++index) {
        Tensor res = model()->resultAt(index);
        Force resultant =
                boost::units::root<2>(
                    boost::units::pow<2>(res.force_x)
                    + boost::units::pow<2>(res.force_y));

        maxAbs = boost::units::fmax(maxAbs, resultant);
    }
    QString params = toString(model()->solverParameters());

    QTreeWidgetItem *item1 = ui->treeWidget->topLevelItem(0); // "Pattern"
    QTreeWidgetItem *item2 = item1->child(0);   // "Solver"
    item2->setText(0, QString("Solver: %0").arg(params));

    QTreeWidgetItem *item3 = item1->child(1);   // "Results"
    QTreeWidgetItem *item4 = item3->child(0);
    item4->setText(0, QString("MAX(ABS(Fxy)) = %0 N").arg( maxAbs.value(), 0, 'f', 1));

}

