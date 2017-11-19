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

#include "splicetoolbar.h"

#include <Core/AbstractSpliceModel>
#include <Core/DesignSpace>
#include <Core/Fastener>

#include <QtCore/QSet>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>

/******************************************************************************
 ******************************************************************************/
SpliceToolbarPrivate::SpliceToolbarPrivate(QWidget *parent)
    : AbstractSpliceView(parent)
    , q_ptr((SpliceToolbar*)parent)
{
}

void SpliceToolbarPrivate::onSelectionFastenerChanged()
{
    q_ptr->_q_selectionFastenerChanged();
}

void SpliceToolbarPrivate::onSelectionDesignSpaceChanged()
{
    q_ptr->_q_selectionDesignSpaceChanged();
}


/******************************************************************************
 ******************************************************************************/
SpliceToolbar::SpliceToolbar(const QString &title, QWidget *parent)
    : QToolBar(title, parent)
    , d_ptr(new SpliceToolbarPrivate(this))
{
    this->createActions();
    this->reset();
}

SpliceToolbar::SpliceToolbar(QWidget *parent)
    : QToolBar(parent)
    , d_ptr(new SpliceToolbarPrivate(this))
{
    this->createActions();
    this->reset();
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

/******************************************************************************
 ******************************************************************************/
void SpliceToolbar::reset()
{
    m_buttonFastenerDuplicate->setEnabled(false);
    m_buttonFastenerSelectAll->setEnabled(false);
    m_buttonFastenerRemove->setEnabled(false);
    // --
    m_buttonDesignSpaceAdd->setVisible(true);
    m_buttonDesignSpaceRemove->setEnabled(false);
    m_buttonDesignSpaceRemove->setVisible(true);
    m_buttonShowDesignSpace->setChecked(true);
    // --
    m_buttonShowAxes->setChecked(true);
    m_buttonShowGrid->setChecked(true);
    m_buttonShowBG->setChecked(false);
    // --
    m_buttonShowComponent->setChecked(false);
    m_buttonShowResultant->setChecked(true);
    m_buttonShowTorque->setChecked(true);
    m_buttonShowLabel->setChecked(false);
    // --
    m_buttonSnap->setChecked(false);
    m_buttonDistance->setChecked(false);
}

/******************************************************************************
 ******************************************************************************/
void SpliceToolbar::createActions()
{
    {
        m_buttonFastenerAdd = new QAction(
                    QIcon(":/icons/menu/bolt_add_24x24.png"),
                    tr("&New Fastener"), this);
        m_buttonFastenerAdd->setStatusTip(tr("Add a new fastener"));
        QObject::connect(m_buttonFastenerAdd, SIGNAL(triggered(bool)),
                         this, SLOT(fastenerAdd()));
        this->addAction(m_buttonFastenerAdd);
    }
    {
        m_buttonFastenerDuplicate = new QAction(
                    QIcon(":/icons/menu/bolt_duplicate_24x24.png"),
                    tr("&Duplicate"), this);
        m_buttonFastenerDuplicate->setStatusTip(tr("Duplicate the selection"));
        m_buttonFastenerDuplicate->setShortcuts(QKeySequence::Paste);
        QObject::connect(m_buttonFastenerDuplicate, SIGNAL(triggered(bool)),
                         this, SLOT(fastenerDuplicate()));
        this->addAction(m_buttonFastenerDuplicate);
    }
    {
        m_buttonFastenerSelectAll = new QAction(
                    QIcon(":/icons/menu/bolt_select_all_24x24.png"),
                    tr("&Select All"), this);
        m_buttonFastenerSelectAll->setStatusTip(tr("Select all the fasteners"));
        m_buttonFastenerSelectAll->setShortcuts(QKeySequence::SelectAll);
        QObject::connect(m_buttonFastenerSelectAll, SIGNAL(triggered(bool)),
                         this, SLOT(fastenerSelectAll()));
        this->addAction(m_buttonFastenerSelectAll);
    }
    {
        m_buttonFastenerRemove = new QAction(
                    QIcon(":/icons/menu/bolt_delete_24x24.png"),
                    tr("&Delete"), this);
        m_buttonFastenerRemove->setStatusTip(tr("Delete the selection"));
        m_buttonFastenerRemove->setShortcuts(QKeySequence::Delete);
        QObject::connect(m_buttonFastenerRemove, SIGNAL(triggered(bool)),
                         this, SLOT(fastenerRemove()));
        this->addAction(m_buttonFastenerRemove);
    }
    this->addSeparator();
    {
        m_buttonShowDesignSpace = new QAction(
                    QIcon(":/icons/menu/edit_design_space_24x24.png"),
                    tr("Show& Design Space"), this);
        m_buttonShowDesignSpace->setCheckable(true);
        m_buttonShowDesignSpace->setStatusTip(tr("Show the design space"));
        QObject::connect(m_buttonShowDesignSpace, SIGNAL(toggled(bool)),
                         SIGNAL(designSpaceVisibilityChanged(bool)));
        this->addAction(m_buttonShowDesignSpace);
    }
    {
        m_buttonDesignSpaceAdd = new QAction(
                    QIcon(":/icons/menu/edit_design_space_add_24x24.png"),
                    tr("&Add Design Space"), this);
        m_buttonDesignSpaceAdd->setStatusTip(tr("Add a new Design Space"));
        QObject::connect(m_buttonDesignSpaceAdd, SIGNAL(triggered(bool)),
                         this, SLOT(designSpaceAdd()));
        this->addAction(m_buttonDesignSpaceAdd);
    }
    {
        m_buttonDesignSpaceRemove = new QAction(
                    QIcon(":/icons/menu/edit_design_space_remove_24x24.png"),
                    tr("&Remove Design Space"), this);
        m_buttonDesignSpaceRemove->setStatusTip(tr("Remove the Design Space"));
        m_buttonDesignSpaceRemove->setShortcuts(QKeySequence::Delete);
        QObject::connect(m_buttonDesignSpaceRemove, SIGNAL(triggered(bool)),
                         this, SLOT(designSpaceRemove()));
        this->addAction(m_buttonDesignSpaceRemove);
    }
    {
        QObject::connect(m_buttonShowDesignSpace, SIGNAL(toggled(bool)),
                         m_buttonDesignSpaceAdd, SLOT(setVisible(bool)));
        QObject::connect(m_buttonShowDesignSpace, SIGNAL(toggled(bool)),
                         m_buttonDesignSpaceRemove, SLOT(setVisible(bool)));
    }
    this->addSeparator();
    {
        m_buttonShowAxes = new QAction(
                    QIcon(":/icons/menu/view_axes_24x24.png"),
                    tr("Show &Axes"), this);
        m_buttonShowAxes->setCheckable(true);
        m_buttonShowAxes->setStatusTip(tr("Show the axes"));
        QObject::connect(m_buttonShowAxes, SIGNAL(toggled(bool)),
                         SIGNAL(axesVisibilityChanged(bool)));
        this->addAction(m_buttonShowAxes);
    }
    {
        m_buttonShowGrid = new QAction(
                    QIcon(":/icons/menu/view_grid_24x24.png"),
                    tr("Show &Grid"), this);
        m_buttonShowGrid->setCheckable(true);
        m_buttonShowGrid->setStatusTip(tr("Show the grid"));
        QObject::connect(m_buttonShowGrid, SIGNAL(toggled(bool)),
                         SIGNAL(gridVisibilityChanged(bool)));
        this->addAction(m_buttonShowGrid);
    }
    {
        m_buttonShowBG = new QAction(
                    QIcon(":/icons/menu/view_background_24x24.png"),
                    tr("Show &Background Image"), this);
        m_buttonShowBG->setCheckable(true);
        m_buttonShowBG->setStatusTip(tr("Show the background image"));
        QObject::connect(m_buttonShowBG,  SIGNAL(toggled(bool)),
                         SIGNAL(imageVisibilityChanged(bool)));
        this->addAction(m_buttonShowBG);
    }
    this->addSeparator();
    {
        m_buttonShowComponent = new QAction(
                    QIcon(":/icons/menu/view_force_component_24x24.png"),
                    tr("Show &Component"), this);
        m_buttonShowComponent->setCheckable(true);
        m_buttonShowComponent->setStatusTip(tr("Show the load component"));
        QObject::connect(m_buttonShowComponent, SIGNAL(toggled(bool)),
                         SIGNAL(componentVisibilityChanged(bool)));
        this->addAction(m_buttonShowComponent);
    }
    {
        m_buttonShowResultant = new QAction(
                    QIcon(":/icons/menu/view_force_resultant_24x24.png"),
                    tr("Show &Resultant"), this);
        m_buttonShowResultant->setCheckable(true);
        m_buttonShowResultant->setStatusTip(tr("Show the load resultant"));
        QObject::connect(m_buttonShowResultant, SIGNAL(toggled(bool)),
                         SIGNAL(resultantVisibilityChanged(bool)));
        this->addAction(m_buttonShowResultant);
    }
    {
        m_buttonShowTorque = new QAction(
                    QIcon(":/icons/menu/view_force_torque_24x24.png"),
                    tr("Show &Torque"), this);
        m_buttonShowTorque->setCheckable(true);
        m_buttonShowTorque->setStatusTip(tr("Show the load torque"));
        QObject::connect(m_buttonShowTorque, SIGNAL(toggled(bool)),
                         SIGNAL(torqueVisibilityChanged(bool)));
        this->addAction(m_buttonShowTorque);
    }
    {
        m_buttonShowLabel = new QAction(
                    QIcon(":/icons/menu/view_labels_24x24.png"),
                    tr("Show Lab&els"), this);
        m_buttonShowLabel->setCheckable(true);
        m_buttonShowLabel->setStatusTip(tr("Show the labels"));
        QObject::connect(m_buttonShowLabel, SIGNAL(toggled(bool)),
                         SIGNAL(labelVisibilityChanged(bool)));
        this->addAction(m_buttonShowLabel);

    }
    this->addSeparator();
    {
        m_buttonSnap = new QAction(
                    QIcon(":/icons/menu/view_snap_24x24.png"),
                    tr("S&nap Objects"), this);
        m_buttonSnap->setCheckable(true);
        m_buttonSnap->setStatusTip(tr("Snap the objects to the grid"));
        QObject::connect(m_buttonSnap, SIGNAL(toggled(bool)),
                         SIGNAL(snapEnabled(bool)));
        this->addAction(m_buttonSnap);

    }
    {
        m_buttonDistance = new QAction(
                    QIcon(":/icons/menu/extra_distance_24x24.png"),
                    tr("Show D&istances"), this);
        m_buttonDistance->setCheckable(true);
        m_buttonDistance->setStatusTip(tr("Show distances between the objects"));
        QObject::connect(m_buttonDistance, SIGNAL(toggled(bool)),
                         SIGNAL(distanceVisibilityChanged(bool)));
        this->addAction(m_buttonDistance);

    }
}


/******************************************************************************
 ******************************************************************************/
bool SpliceToolbar::isDesignSpaceVisible() const
{
    return m_buttonShowDesignSpace->isChecked();
}

void SpliceToolbar::setDesignSpaceVisible(bool visible)
{
    m_buttonShowDesignSpace->setChecked(visible);
}

bool SpliceToolbar::isAxesVisible() const
{
    return m_buttonShowAxes->isChecked();
}

void SpliceToolbar::setAxesVisible(bool visible)
{
    m_buttonShowAxes->setChecked(visible);
}

bool SpliceToolbar::isGridVisible() const
{
    return m_buttonShowGrid->isChecked();
}

void SpliceToolbar::setGridVisible(bool visible)
{
    m_buttonShowGrid->setChecked(visible);
}

bool SpliceToolbar::isImageVisible() const
{
    return m_buttonShowBG->isChecked();
}

void SpliceToolbar::setImageVisible(bool visible)
{
    m_buttonShowBG->setChecked(visible);
}

bool SpliceToolbar::isComponentVisible() const
{
    return m_buttonShowComponent->isChecked();
}

void SpliceToolbar::setComponentVisible(bool visible)
{
    m_buttonShowComponent->setChecked(visible);
}

bool SpliceToolbar::isResultantVisible() const
{
    return m_buttonShowResultant->isChecked();
}

void SpliceToolbar::setResultantVisible(bool visible)
{
    m_buttonShowResultant->setChecked(visible);
}

bool SpliceToolbar::isTorqueVisible() const
{
    return m_buttonShowTorque->isChecked();
}

void SpliceToolbar::setTorqueVisible(bool visible)
{
    m_buttonShowTorque->setChecked(visible);
}

bool SpliceToolbar::isLabelVisible() const
{
    return m_buttonShowLabel->isChecked();
}

void SpliceToolbar::setLabelVisible(bool visible)
{
    m_buttonShowLabel->setChecked(visible);
}

bool SpliceToolbar::isSnapEnable() const
{
    return m_buttonSnap->isChecked();
}

void SpliceToolbar::setSnapEnable(bool enable)
{
    m_buttonSnap->setChecked(enable);
}

bool SpliceToolbar::isDistanceVisible() const
{
    return m_buttonDistance->isChecked();
}

void SpliceToolbar::setDistanceVisible(bool visible)
{
    m_buttonDistance->setChecked(visible);
}

/******************************************************************************
 ******************************************************************************/
void SpliceToolbar::fastenerAdd()
{
    Fastener f(0.0*_mm, 0.0*_mm, 4.78*_mm, 3.*_mm);
    int count =  model()->fastenerCount();
    model()->insertFastener(count, f);
}

void SpliceToolbar::fastenerDuplicate()
{
    QSet<int> newSet;
    QSet<int> set = model()->selectedFastenerIndexes();
    QSetIterator<int> it(set);
    while (it.hasNext()) {
        int index = it.next();
        int newIndex = model()->fastenerCount();
        Fastener newFastener( model()->fastenerAt(index) );
        model()->insertFastener(newIndex, newFastener);
        newSet << newIndex;
    }
    model()->setFastenerSelection(newSet);
}

void SpliceToolbar::fastenerSelectAll()
{
    QSet<int> set;
    int count =  model()->fastenerCount();
    while (count >0) {
        count--;
        set << count;
    }
    model()->setFastenerSelection(set);
}

void SpliceToolbar::fastenerRemove()
{
    /*
     * REMARK: Here the selection changes during the removal, thus
     * we need to remove the indexes from the highest to the lowest.
     */
    QSet<int> set = model()->selectedFastenerIndexes();
    QList<int> list = set.toList();
    qSort(list);
    while (!list.isEmpty()) {
        int index = list.last();
        list.removeLast();
        model()->removeFastener(index);
    }
}


/******************************************************************************
 ******************************************************************************/
void SpliceToolbar::designSpaceAdd()
{
    const qreal halfwidth = 0.010; // meters
    DesignSpace ds;
    ds.polygon << QPointF( -halfwidth, -halfwidth)
               << QPointF( -halfwidth,  halfwidth)
               << QPointF(  halfwidth,  halfwidth)
               << QPointF(  halfwidth, -halfwidth);
    int count =  model()->designSpaceCount();
    model()->insertDesignSpace(count, ds);
}

void SpliceToolbar::designSpaceRemove()
{
    /*
     * REMARK: Here the selection changes during the removal, thus
     * we need to remove the indexes from the highest to the lowest.
     */
    QSet<int> set = model()->selectedDesignSpaceIndexes();
    QList<int> list = set.toList();
    qSort(list);
    while (!list.isEmpty()) {
        int index = list.last();
        list.removeLast();
        model()->removeDesignSpace(index);
    }
}

/******************************************************************************
 ******************************************************************************/
void SpliceToolbar::_q_selectionFastenerChanged()
{
    bool selected = (!model()->selectedFastenerIndexes().isEmpty());
    m_buttonFastenerDuplicate->setEnabled(selected);
    m_buttonFastenerRemove->setEnabled(selected);
}

void SpliceToolbar::_q_selectionDesignSpaceChanged()
{
    bool selected = (!model()->selectedDesignSpaceIndexes().isEmpty());
    m_buttonDesignSpaceRemove->setEnabled(selected);
}
