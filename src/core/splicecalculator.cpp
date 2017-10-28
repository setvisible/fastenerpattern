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

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>


/*! \class SpliceCalculator
 *  \brief The class SpliceCalculator is the main manager for a splice document.
 *
 */

/*! \brief Constructor.
 */
SpliceCalculator::SpliceCalculator(QObject *parent) : AbstractSpliceModel(parent)
  , m_params(SolverParameters::NoSolver)
  , m_solver(Q_NULLPTR)
  , m_splice(new Splice(this))
{
    this->clear();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Clear and emit the change, in order to update the views,
 * that derive from AbstractSpliceView.
 */
void SpliceCalculator::clear()
{
    m_splice->setTitle(QStringLiteral("untitled"));
    m_splice->setAuthor(QStringLiteral("-"));
    m_splice->setDate(QStringLiteral("-"));
    m_splice->setDescription(QString());

    QSet<int> emptySet;
    setSelection(emptySet);

    setAppliedLoad(Tensor(0*N, 0*N, 0*N_m));

    int i = fastenerCount();
    while (i > 0) {
        i--;
        removeFastener(i);
    }
}

/******************************************************************************
 ******************************************************************************/
QString SpliceCalculator::title() const
{
    return m_splice->title();
}

void SpliceCalculator::setTitle(const QString &title)
{
    if (m_splice->title() != title) {
        m_splice->setTitle(title);
        emit changed();
    }
}

/******************************************************************************
 ******************************************************************************/
QString SpliceCalculator::author() const
{
    return m_splice->author();
}

void SpliceCalculator::setAuthor(const QString &author)
{
    if (m_splice->author() != author) {
        m_splice->setAuthor(author);
        emit changed();
    }
}

/******************************************************************************
 ******************************************************************************/
QString SpliceCalculator::date() const
{
    return m_splice->date();
}

void SpliceCalculator::setDate(const QString &date)
{
    if (m_splice->date() != date) {
        m_splice->setDate(date);
        emit changed();
    }
}

/******************************************************************************
 ******************************************************************************/
QString SpliceCalculator::description() const
{
    return m_splice->description();
}

void SpliceCalculator::setDescription(const QString &description)
{
    if (m_splice->description() != description) {
        m_splice->setDescription(description);
        emit changed();
    }
}

/******************************************************************************
 ******************************************************************************/
/* SERIALISATION JSON */
/*! \brief Assign the SpliceCalculator's members values from the given \a json object.
 */
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

/*! \brief Assigns the values from the SpliceCalculator to the given \a json object.
 */
void SpliceCalculator::write(QJsonObject &json) const
{
    m_splice->write(json);
}

/******************************************************************************
 ******************************************************************************/
int SpliceCalculator::fastenerCount() const
{
    return m_splice->fastenerCount();
}

Fastener SpliceCalculator::fastenerAt(const int index) const
{
    if (index >= 0 && index < m_splice->fastenerCount()) {
        return m_splice->fastenerAt(index);
    }
    return Fastener();
}

Tensor SpliceCalculator::appliedLoad() const
{
    return m_splice->appliedLoad();
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

/******************************************************************************
 ******************************************************************************/
bool SpliceCalculator::insertFastener(const int index, const Fastener &fastener)
{
    m_splice->insertFastener(index, fastener);
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
    if (index >= 0 && index < m_splice->fastenerCount()) {
        m_splice->removeFastenerAt(index);
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
    if (index < 0 || index >= m_splice->fastenerCount())
        return false;

    const Fastener old = m_splice->fastenerAt(index);
    if (old == fastener)
        return false;

    m_splice->setFastenerAt(index, fastener);

    emit fastenersChanged(index, fastener);
    emit changed();
    recalculate();
    emit resultsChanged();
    return true;
}

bool SpliceCalculator::setAppliedLoad(const Tensor &loadcase)
{
    if (m_splice->appliedLoad() == loadcase)
        return false;
    m_splice->setAppliedLoad(loadcase);
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

bool SpliceCalculator::setSolverParameters(SolverParameters params)
{
    if (m_params == params)
        return false;

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

    case SolverParameters::FiniteElementSolver:
        /* TO DO */
        break;
    case SolverParameters::NoSolver:
    default:
        break;
    }
    m_params = params;
    recalculate();
    emit resultsChanged();
    return true;
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

