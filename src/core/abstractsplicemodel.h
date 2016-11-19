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

#ifndef CORE_ABSTRACT_SPLICE_MODEL_H
#define CORE_ABSTRACT_SPLICE_MODEL_H

#include <QtCore/QObject>
#include <QtCore/QSet>

#include <Core/Tensor>
#include <Core/Fastener>

class AbstractSpliceModel : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSpliceModel(QObject *parent = 0) : QObject(parent) {}
    ~AbstractSpliceModel() {}

    Q_INVOKABLE virtual int fastenerCount() const = 0;
    Q_INVOKABLE virtual Fastener fastenerAt(const int index) const = 0;
    Q_INVOKABLE virtual Tensor appliedLoad() const = 0;
    Q_INVOKABLE virtual Tensor resultAt(const int index) const = 0;
    Q_INVOKABLE virtual QSet<int> selectedIndexes() const = 0;

Q_SIGNALS:
    void appliedLoadChanged();
    void fastenersInserted(const int index, const Fastener &fastener);
    void fastenersChanged(const int index, const Fastener &fastener);
    void fastenersRemoved(const int index);
    void selectionChanged();
    void resultsChanged();

public Q_SLOTS:

    Q_INVOKABLE virtual bool insertFastener(const int index, const Fastener &fastener) {
        Q_UNUSED(index);
        Q_UNUSED(fastener);
        return false;
    }
    Q_INVOKABLE virtual bool removeFastener(const int index) {
        Q_UNUSED(index);
        return false;
    }

    Q_INVOKABLE virtual bool setFastener(const int index, const Fastener &fastener) {
        Q_UNUSED(index);
        Q_UNUSED(fastener);
        return false;
    }

    Q_INVOKABLE virtual bool setAppliedLoad(const Tensor &loadcase) {
        Q_UNUSED(loadcase);
        return false;
    }

    Q_INVOKABLE virtual bool setSelection(const QSet<int> indexes) {
        Q_UNUSED(indexes);
        return false;
    }


};

#endif // CORE_ABSTRACT_SPLICE_MODEL_H
