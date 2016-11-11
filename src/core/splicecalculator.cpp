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

#include "splicecalculator.h"

#include <Core/Solvers/ISolver>
#include <Core/Solvers/RigidBodySolver>

#include "splice.h"
#include "fastener.h"
#include "tensor.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>


SpliceCalculator::SpliceCalculator(QObject *parent) : AbstractSpliceModel(parent)
  , m_params(SolverParameters::NoSolver)
  , m_solver(Q_NULLPTR)
  , m_splice(new Splice(this))
{
    this->clear();
}

void SpliceCalculator::clear()
{
    m_splice->m_name = "untitled";
    setAppliedLoad(Tensor(0*N, 0*N, 0*N_m));

    int i = fastenerCount();
    while (i > 0) {
        i--;
        removeFastener(i);
    }
}


/**********************************************************************
 **********************************************************************/
/* SERIALISATION JSON */
void SpliceCalculator::read(const QJsonObject &json)
{
    clear();
    m_splice->read(json);
    emit appliedLoadChanged();
    for (int i = 0 ; i < fastenerCount(); ++i) {
        emit fastenersInserted(i, fastenerAt(i));
    }
    emit changed();
    recalculate();
    emit resultsChanged();
}

void SpliceCalculator::write(QJsonObject &json) const
{
    m_splice->write(json);
}

/**********************************************************************
 **********************************************************************/

int SpliceCalculator::fastenerCount() const
{
    return m_splice->m_fasteners.count();
}

Fastener SpliceCalculator::fastenerAt(const int index) const
{
    if (index >= 0 && index < m_splice->m_fasteners.count()) {
        return m_splice->m_fasteners.at(index);
    }
    return Fastener();
}

Tensor SpliceCalculator::loadcase() const
{
    return m_splice->m_appliedLoad;
}

Tensor SpliceCalculator::resultAt(const int index) const
{
    if (index >= 0 && index < m_results.count()) {
        return m_results.at(index);
    }
    return Tensor();
}

QSet<int> SpliceCalculator::selectedIndexes() const
{
    return m_selectedIndexes;
}

/**********************************************************************
 **********************************************************************/

bool SpliceCalculator::insertFastener(const int index, const Fastener &fastener)
{
    m_splice->m_fasteners.insert(index, fastener);
    emit fastenersInserted(index, fastener);
    emit changed();
    recalculate();
    emit resultsChanged();
    return true;
}

bool SpliceCalculator::removeFastener(const int index)
{
    if (m_selectedIndexes.remove( index )) {
        emit selectionChanged();
    }
    if (index >= 0 && index < m_splice->m_fasteners.count()) {
        m_splice->m_fasteners.removeAt(index);
        emit fastenersRemoved(index);
        emit changed();
        recalculate();
        emit resultsChanged();
        return true;
    }
    return false;
}

bool SpliceCalculator::setFastener(const int index, const Fastener &fastener)
{
    if (index < 0 || index >= m_splice->m_fasteners.count())
        return false;

    const Fastener old = m_splice->m_fasteners.at(index);
    if (old == fastener)
        return false;

    /// \todo intead of m_splice->m_fasteners[index].swap(fastener);
    m_splice->m_fasteners[index] = fastener;

    emit fastenersChanged(index, fastener);
    emit changed();
    recalculate();
    emit resultsChanged();
    return true;
}

bool SpliceCalculator::setAppliedLoad(const Tensor &loadcase)
{
    if (m_splice->m_appliedLoad == loadcase)
        return false;
    m_splice->m_appliedLoad = loadcase;
    emit appliedLoadChanged();
    emit changed();
    recalculate();
    emit resultsChanged();
    return true;
}

bool SpliceCalculator::setSelection(const QSet<int> indexes)
{
    if (m_selectedIndexes == indexes)
        return false;
    m_selectedIndexes = indexes;
    emit selectionChanged();
    return true;
}

SolverParameters SpliceCalculator::solverParameters() const
{
    return m_params;
}

void SpliceCalculator::setSolverParameters(SolverParameters params)
{
    if (m_params == params)
        return;

    if (m_solver) {
        delete m_solver;
        m_solver = Q_NULLPTR;
    }

    switch(params) {
    case SolverParameters::RigidBodySolverWithIsoBearing:
    {
        RigidBodySolver* s = new RigidBodySolver(this);
        s->setParameters( SolverParameters::RigidBodySolverWithIsoBearing );
        m_solver = (ISolver*)(s);
        break;
    }

    case SolverParameters::RigidBodySolverWithIsoShear:
    {
        RigidBodySolver* s = new RigidBodySolver(this);
        s->setParameters( SolverParameters::RigidBodySolverWithIsoShear );
        m_solver = (ISolver*)(s);
        break;
    }

    case SolverParameters::NoSolver:
    case SolverParameters::FiniteElementSolver:
    case SolverParameters::OptimisationSolver:
    default:
        break;
    }
    m_params = params;
    recalculate();
    emit resultsChanged();
}

void SpliceCalculator::recalculate()
{
    /// \todo Use worker thread here.
    /// \todo see  Mandelbrot Example  or  Blocking Fortune Client Example
    if (m_solver && m_splice) {
        m_results = m_solver->calculate( m_splice );
        return;
    }
    m_results.clear();
}

