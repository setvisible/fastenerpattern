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

#include <Core/DesignSpace>
#include <Core/Fastener>
#include <Core/Tensor>
#include <Core/Solvers/Parameters>

#include <QtCore/QObject>
#include <QtCore/QtContainerFwd> /* Forward Declarations of the Qt's Containers */

class DesignSpace;
class Fastener;
class Tensor;
enum class SolverParameters;

class AbstractSpliceModel : public QObject
{
    Q_OBJECT

public:
    explicit AbstractSpliceModel(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    virtual ~AbstractSpliceModel() Q_DECL_NOEXCEPT {}

    /* Public Getters */
    Q_INVOKABLE virtual int fastenerCount() const = 0;
    Q_INVOKABLE virtual Fastener fastenerAt(const int index) const = 0;

    Q_INVOKABLE virtual int designSpaceCount() const = 0;
    Q_INVOKABLE virtual DesignSpace designSpaceAt(const int index) const = 0;

    Q_INVOKABLE virtual QSet<int> selectedFastenerIndexes() const = 0;
    Q_INVOKABLE virtual QSet<int> selectedDesignSpaceIndexes() const = 0;

    Q_INVOKABLE virtual Tensor appliedLoad() const = 0;
    Q_INVOKABLE virtual Tensor resultAt(const int index) const = 0;
    Q_INVOKABLE virtual SolverParameters solverParameters() const = 0;


Q_SIGNALS:

    /* Internal Notifications */
    void fastenerInserted(const int index, const Fastener &fastener);
    void fastenerChanged(const int index, const Fastener &fastener);
    void fastenerRemoved(const int index);

    void designSpaceInserted(const int index, const DesignSpace &designSpace);
    void designSpaceChanged(const int index, const DesignSpace &designSpace);
    void designSpaceRemoved(const int index);

    void selectionFastenerChanged();
    void selectionDesignSpaceChanged();

    void appliedLoadChanged();
    void resultsChanged();


public Q_SLOTS:

    /* Public Setters */
    Q_INVOKABLE virtual bool insertFastener(const int index, const Fastener &fastener) {
        Q_UNUSED(index);
        Q_UNUSED(fastener);
        return false;
    }

    Q_INVOKABLE virtual bool setFastener(const int index, const Fastener &fastener) {
        Q_UNUSED(index);
        Q_UNUSED(fastener);
        return false;
    }

    Q_INVOKABLE virtual bool removeFastener(const int index) {
        Q_UNUSED(index);
        return false;
    }

    Q_INVOKABLE virtual bool insertDesignSpace(const int index, const DesignSpace &designSpace) {
        Q_UNUSED(index);
        Q_UNUSED(designSpace);
        return false;
    }

    Q_INVOKABLE virtual bool setDesignSpace(const int index, const DesignSpace &designSpace) {
        Q_UNUSED(index);
        Q_UNUSED(designSpace);
        return false;
    }

    Q_INVOKABLE virtual bool removeDesignSpace(const int index) {
        Q_UNUSED(index);
        return false;
    }

    Q_INVOKABLE virtual bool setAppliedLoad(const Tensor &loadcase) {
        Q_UNUSED(loadcase);
        return false;
    }

    Q_INVOKABLE virtual bool setFastenerSelection(const QSet<int> indexes);

    Q_INVOKABLE virtual bool setDesignSpaceSelection(const QSet<int> indexes);

    Q_INVOKABLE virtual bool setSolverParameters(SolverParameters params) {
        Q_UNUSED(params);
        return false;
    }

};

#endif // CORE_ABSTRACT_SPLICE_MODEL_H
