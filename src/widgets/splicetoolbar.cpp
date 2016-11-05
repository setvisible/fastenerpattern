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

#include "splicetoolbar.h"

SpliceToolbarPrivate::SpliceToolbarPrivate(QWidget *parent)
    : AbstractSpliceView(parent)
    , q_ptr((SpliceToolbar*)parent)
{
}

void SpliceToolbarPrivate::selectionChanged()
{
    q_ptr->_q_selectionChanged();
}


/***********************************************************************************
 ***********************************************************************************/
SpliceToolbar::SpliceToolbar(const QString &title, QWidget *parent)
    : QToolBar(title, parent)
    , d_ptr(new SpliceToolbarPrivate(this))
{
    this->createActions();
}

SpliceToolbar::SpliceToolbar(QWidget *parent)
    : QToolBar(parent)
    , d_ptr(new SpliceToolbarPrivate(this))
{
    this->createActions();
}

SpliceToolbar::~SpliceToolbar()
{
    if (d_ptr)
        delete d_ptr;
}

AbstractSpliceModel *SpliceToolbar::model() const
{
    return d_ptr->model();
}
void SpliceToolbar::setModel(AbstractSpliceModel *model)
{
    d_ptr->setModel(model);
}

void SpliceToolbar::createActions()
{
    {
        QAction *buttonAdd = new QAction(
                    QIcon(":/icons/menu/bolt_add_24x24.png"),
                    tr("&New Fastener"), this);
        buttonAdd->setStatusTip(tr("Add a new fastener"));
        QObject::connect(buttonAdd, SIGNAL(triggered(bool)), this, SLOT(add()));
        this->addAction(buttonAdd);
    }
    {
        m_buttonCopy = new QAction(
                    QIcon(":/icons/menu/bolt_duplicate_24x24.png"),
                    tr("&Duplicate"), this);
        m_buttonCopy->setStatusTip(tr("Duplicate the selection"));
        m_buttonCopy->setShortcuts(QKeySequence::Copy);
        QObject::connect(m_buttonCopy, SIGNAL(triggered(bool)), this, SLOT(duplicate()));
        this->addAction(m_buttonCopy);
    }
    {
        QAction *buttonSelectAll = new QAction(
                    QIcon(":/icons/menu/bolt_select_all_24x24.png"),
                    tr("&Select All"), this);
        buttonSelectAll->setStatusTip(tr("Select all the fasteners"));
        buttonSelectAll->setShortcuts(QKeySequence::SelectAll);
        QObject::connect(buttonSelectAll, SIGNAL(triggered(bool)), this, SLOT(selectAll()));
        this->addAction(buttonSelectAll);
    }
    {
        m_buttonRemove = new QAction(
                    QIcon(":/icons/menu/bolt_delete_24x24.png"),
                    tr("&Delete"), this);
        m_buttonRemove->setStatusTip(tr("Delete the selection"));
        m_buttonRemove->setShortcuts(QKeySequence::Delete);
        QObject::connect(m_buttonRemove, SIGNAL(triggered(bool)), this, SLOT(remove()));
        this->addAction(m_buttonRemove);
    }
    this->addSeparator();
    {
        m_buttonAxes = new QAction(
                    QIcon(":/icons/menu/view_axes_24x24.png"),
                    tr("Show &Axes"), this);
        m_buttonAxes->setCheckable(true);
        m_buttonAxes->setStatusTip(tr("Set the axes visible"));
        QObject::connect(m_buttonAxes, SIGNAL(toggled(bool)), SIGNAL(axesVisibilityChanged(bool)));
        this->addAction(m_buttonAxes);
    }
    {
        m_buttonGrid = new QAction(
                    QIcon(":/icons/menu/view_grid_24x24.png"),
                    tr("Show &Grid"), this);
        m_buttonGrid->setCheckable(true);
        m_buttonGrid->setStatusTip(tr("Set the grid visible"));
        QObject::connect(m_buttonGrid, SIGNAL(toggled(bool)), SIGNAL(gridVisibilityChanged(bool)));
        this->addAction(m_buttonGrid);
    }
    {
        m_buttonBG = new QAction(
                    QIcon(":/icons/menu/view_background_24x24.png"),
                    tr("Show &Background Image"), this);
        m_buttonBG->setCheckable(true);
        m_buttonBG->setStatusTip(tr("Set the background image visible"));
        QObject::connect(m_buttonBG, SIGNAL(toggled(bool)), SIGNAL(imageVisibilityChanged(bool)));
        this->addAction(m_buttonBG);
    }
}


/***********************************************************************************
 ***********************************************************************************/

bool SpliceToolbar::isAxesVisible() const
{
    return m_buttonAxes->isChecked();
}

void SpliceToolbar::setAxesVisible(bool visible)
{
    m_buttonAxes->setChecked(visible);
}

bool SpliceToolbar::isGridVisible() const
{
    return m_buttonGrid->isChecked();
}

void SpliceToolbar::setGridVisible(bool visible)
{
    m_buttonGrid->setChecked(visible);
}

bool SpliceToolbar::isImageVisible() const
{
    return m_buttonBG->isChecked();
}

void SpliceToolbar::setImageVisible(bool visible)
{
    m_buttonBG->setChecked(visible);
}


/***********************************************************************************
 ***********************************************************************************/
void SpliceToolbar::add()
{
    Fastener f(0.0*mm, 0.0*mm, 4.78*mm, 3.*mm);
    int count =  model()->fastenerCount();
    model()->insertFastener(count, f);
}

void SpliceToolbar::duplicate()
{
    QSet<int> newSet;
    QSet<int> set = model()->selectedIndexes();
    QSetIterator<int> it(set);
    while (it.hasNext()) {
        int index = it.next();
        int newIndex = model()->fastenerCount();
        Fastener newFastener( model()->fastenerAt(index) );
        model()->insertFastener(newIndex, newFastener);
        newSet << newIndex;
    }
    model()->setSelection(newSet);
}

void SpliceToolbar::selectAll()
{
    QSet<int> set;
    int count =  model()->fastenerCount();
    while (count >0) {
        count--;
        set << count;
    }
    model()->setSelection(set);
}

void SpliceToolbar::remove()
{
    /*
     * REMARK: Here the selection changes during the removal, thus
     * we need to remove the indexes from the highest to the lowest.
     */
    QSet<int> set = model()->selectedIndexes();
    QList<int> list = set.toList();
    qSort(list);
    while (!list.isEmpty()) {
        int index = list.last();
        list.removeLast();
        model()->removeFastener(index);
    }
}

void SpliceToolbar::_q_selectionChanged()
{
    bool selected = (!model()->selectedIndexes().isEmpty());
    m_buttonCopy->setEnabled(selected);
    m_buttonRemove->setEnabled(selected);
}


