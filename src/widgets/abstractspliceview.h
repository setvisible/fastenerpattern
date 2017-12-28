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

#ifndef WIDGETS_ABSTRACT_SPLICE_VIEW_H
#define WIDGETS_ABSTRACT_SPLICE_VIEW_H

#include <QtWidgets/QWidget>

class AbstractSpliceModel;
class DelayTimer;
class DesignSpace;
class Fastener;

class AbstractSpliceView : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSpliceView(QWidget *parent = Q_NULLPTR);
    virtual ~AbstractSpliceView() Q_DECL_NOEXCEPT {}

    virtual void setModel(AbstractSpliceModel *model);
    AbstractSpliceModel *model() const;

public Q_SLOTS:
    virtual void onFastenerInserted(const int index, const Fastener &fastener);
    virtual void onFastenerChanged(const int index, const Fastener &fastener);
    virtual void onFastenerRemoved(const int index);

    virtual void onDesignSpaceInserted(const int index, const DesignSpace &designSpace);
    virtual void onDesignSpaceChanged(const int index, const DesignSpace &designSpace);
    virtual void onDesignSpaceRemoved(const int index);

    virtual void onAppliedLoadChanged();
    virtual void onResultsChanged();

    virtual void onSelectionFastenerChanged();
    virtual void onSelectionDesignSpaceChanged();

protected:
    int updateDelay() const;
    void setUpdateDelay(const int msec);

private:
    AbstractSpliceModel *m_model;
    DelayTimer *m_delayTimer;
};

#endif // WIDGETS_ABSTRACT_SPLICE_VIEW_H
