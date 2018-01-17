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

#include "optimisationsolver.h"
#include "maxminload.h"

#include <Core/Solvers/ISolver>
#include <Core/Tensor>
#include <Core/Splice>
#include <Math/Utils>

#include <QtCore/QDebug>
#include <QtGui/QPainterPath>


/*! \class OptimisationSolver
 * \brief The class OptimisationSolver is in charge of global optimum search.
 *
 * The search can be done in the current thread in a synchronous manner
 * with runSync(), or in an asynchronous manner with runAsync()
 * though an(some) other thread(s).
 *
 * OptimisationSolver methods MUST be thread-safe (and then reentrant).
 * Indeed, OptimisationSolver::runAsync() and OptimisationSolver::runSync()
 * can be run by several threads, at the same time.
 *
 * That is, class members of OptimisationSolver are protected by mutexes
 * (\a QMutex or \a QReadWriteLock).
 *
 * \sa Controller
 */
OptimisationSolver::OptimisationSolver(QObject *parent) : QObject(parent)
  , m_solver(Q_NULLPTR)
  , m_input(Q_NULLPTR)
  , m_output(Q_NULLPTR)
  , m_objective(OptimisationDesignObjective::MinimizeMaxLoad)
  , m_constraints(OptimisationDesignConstraint::NoConstraint)
  , m_randomIterations(100)
  , m_localIterations(10)
{
}

/*! \brief Destructor.
 * OptimisationSolver is not responsible for the destruction
 * of its children returned by \a solver(), \a input() and \a output().
 * Thus their pointers are just nulled, not deleted.
 */
OptimisationSolver::~OptimisationSolver()
{
    m_solver = Q_NULLPTR;
    m_input  = Q_NULLPTR;
    m_output = Q_NULLPTR;
}

/******************************************************************************
 ******************************************************************************/
ISolver *OptimisationSolver::solver() const
{
    return m_solver;
}

void OptimisationSolver::setSolver(ISolver *solver)
{
    m_solver = solver;
}

/******************************************************************************
 ******************************************************************************/
Splice *OptimisationSolver::input() const
{
    return m_input;
}

void OptimisationSolver::setInput(Splice *input)
{
    m_input = input;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Set the pointer to the output splice object.
 * It represents the best solution found after the computation.
 */
Splice *OptimisationSolver::output() const
{
    return m_output;
}

void OptimisationSolver::setOutput(Splice *output)
{ 
    m_output = output;
}

/******************************************************************************
 ******************************************************************************/
OptimisationDesignObjective OptimisationSolver::objective() const
{
    return m_objective;
}

void OptimisationSolver::setDesignObjective(OptimisationDesignObjective objective)
{
    m_objective = objective;
}

/******************************************************************************
 ******************************************************************************/
OptimisationDesignConstraints OptimisationSolver::constraints() const
{
    return m_constraints;
}

void OptimisationSolver::setDesignConstraints(OptimisationDesignConstraints constraints)
{
    m_constraints = constraints;
}

/******************************************************************************
 ******************************************************************************/
int OptimisationSolver::randomIterations() const
{
    return m_randomIterations;
}

void OptimisationSolver::setRandomIterations(const int iterations)
{
    m_randomIterations = iterations;
}


/******************************************************************************
 ******************************************************************************/

/*  HACKS -- HACKS -- HACKS -- HACKS -- HACKS */
static inline void designSpacesCopy(const Splice *from, Splice *to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);
    to->removeAllDesignSpaces();
    for (int i = 0; i < from->designSpaceCount(); ++i) {
        to->addDesignSpace(from->designSpaceAt(i));
    }

}
/*  HACKS -- HACKS -- HACKS -- HACKS -- HACKS */
static inline void fastenersCopy(const Splice *from, Splice *to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);
    to->removeAllFasteners();
    for (int i = 0; i < from->fastenerCount(); ++i) {
        to->addFastener(from->fastenerAt(i));
    }

}

/*  HACKS -- HACKS -- HACKS -- HACKS -- HACKS */
static inline void deepCopy(const Splice *from, Splice *to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);

    to->setTitle(from->title());
    to->setAuthor(from->author());
    to->setDate(from->date());
    to->setDescription(from->description());

    to->setAppliedLoad(from->appliedLoad());

    fastenersCopy(from, to);
    designSpacesCopy(from, to);
}

/******************************************************************************
 ******************************************************************************/
bool OptimisationSolver::sanitarize()
{
    if (m_localIterations < 0) {
        m_localIterations = 1;
    }
    if (m_localIterations > 10) {
        m_localIterations = 10;
    }

    bool isValid = true;
    if (!m_solver) {
        emit errorDetected(OptimisationErrorType::ERR_UNDEFINED_SOLVER);
        isValid = false;
    }
    if (!m_input) {
        emit errorDetected(OptimisationErrorType::ERR_UNDEFINED_INPUT_SPLICE);
        isValid = false;
    }
    if (!m_output) {
        emit errorDetected(OptimisationErrorType::ERR_UNDEFINED_OUTPUT_SPLICE);
        isValid = false;
    }
    if (m_input) {
        if (m_input->appliedLoad() == Tensor()) {
            emit errorDetected(OptimisationErrorType::ERR_NO_APPLIED_LOAD);
            isValid = false;
        }
        if (m_input->designSpaceCount() == 0) {
            emit errorDetected(OptimisationErrorType::ERR_NO_DESIGNSPACE);
            isValid = false;
        }
        if (m_input->fastenerCount() == 0) {
            emit errorDetected(OptimisationErrorType::ERR_NO_FASTENER);
            isValid = false;
        }
    }
    return isValid;
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Precompute some invariants.
 *
 * Invariants are values that are needed by the solver, but can be computed
 * only once (precomputed), so moved outside the solving loop.
 *
 * The Invariants don't change during the computation (e.g. design spaces...).
 *
 * \sa postcompute()
 */
void OptimisationSolver::precompute()
{
    m_lock.lockForWrite();

    // qDebug() << "Precomputing the global design space...";
    m_precomputedArea.clear();
    for (int i = 0; i < m_input->designSpaceCount(); ++i) {
        DesignSpace ds = m_input->designSpaceAt(i);
        m_precomputedArea = m_precomputedArea.united( ds.polygon );
    }
    // #ifdef QT_DEBUG
    //     qDebug() << "Precomputed design space:";
    //     for (int i = 0; i < m_precomputedArea.count(); ++i) {
    //         qDebug() << m_precomputedArea.at(i);
    //     }
    // #endif

    deepCopy(m_input, m_output);

    m_lock.unlock();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Precompute some invariants.
 *
 * Basically, it clears the invariants.
 *
 * \sa precompute()
 */
void OptimisationSolver::postcompute()
{
    m_lock.lockForWrite();
    m_precomputedArea.clear();
    m_lock.unlock();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Start the entire global optimum search.
 *
 * This is a blocking method (synchronous).
 *
 * The implementation of this method was mainly driven
 * for synchronous unit tests. Prefer \a runAsync().
 *
 * \sa runAsync()
 */
void OptimisationSolver::runSync()
{
    if (!sanitarize()) {
        return;
    }
    precompute();
    runAsync();
    postcompute();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Start the global optimum search.
 *
 * This is a blocking method (synchronous).
 *
 * This method does the same as \a runSync(),
 * but \a precompute() must be called before this method,
 * and \a postcompute() must be called after this method.
 *
 * Note that this method performs one unique optimisation loop.
 * This loop is just a part of the entire optimisation.
 * We should call this method more than thousand times to perform the entire optimisation.
 *
 * Since it's a expensive task, it's strongly recommanded
 * to start this method from another thread, e.g. a QThread worker thread.
 *
 * \sa runSync()
 */
void OptimisationSolver::runAsync()
{
    Q_ASSERT(m_solver);
    Q_ASSERT(m_input);
    Q_ASSERT(m_output);

    QPolygonF area;
    Splice bestSolution;

    m_lock.lockForRead();
    area = m_precomputedArea;
    deepCopy( m_output, &bestSolution );
    m_lock.unlock();

    QList<Tensor> result = m_solver->calculate( &bestSolution );
    Force bestResultantForce = maxLoad(result);
    result.clear();

    Q_ASSERT(bestResultantForce.value() > 0.0);

    /* Random Search (global optimisation) */
    for (int i = 0; i < m_randomIterations; ++i) {

        Splice solution;
        deepCopy( &bestSolution, &solution );

        bool ok = randomizePosition( &solution );
        if (!ok) {
            continue;
        }

        /* Local Mininum Search (local optimisation) */
        for (int j = 0; j < m_localIterations; ++j) {

            // delta is between 0.100 and 0.000098 meter --> logarithmic precision
            qreal delta = 0.100 / qPow(2, j); // in meter

            /* We test points near the current solution, in order to find a local minimum */
            for (int k = 0; k < solution.fastenerCount(); ++k) {

                const int size = 2;
                Q_ASSERT(size>0);

                /* Example: 'size' == 1
                 * --------+--------+--------
                 *  -1,-1  |  0,-1  |  1,-1
                 *         |        |
                 * --------+--------+--------
                 *  -1, 0  |  0, 0  |  1, 0
                 *         |        |
                 * --------+--------+--------
                 *  -1, 1  |  0, 1  |  1, 1
                 *         |        |
                 * --------+--------+--------
                 */

                for (int ii = -size; ii < size; ++ii) {
                    for (int jj = -size; jj < size; ++jj) {
                        if (ii == 0 && jj == 0) {
                            continue;
                        }

                        qreal deltaX = (qreal)ii / (qreal)size * delta;
                        qreal deltaY = (qreal)jj / (qreal)size * delta;

                        Splice localSolution;
                        deepCopy( &solution, &localSolution );

                        Fastener fk = localSolution.fastenerAt(k);

                        QPointF proposedPoint;
                        proposedPoint = QPointF(
                                    fk.positionX.value() + deltaX,
                                    fk.positionY.value() + deltaY);


                        if (! area.containsPoint(proposedPoint, Qt::WindingFill)) { // OddEvenFill
                            continue;
                        }

                        fk.positionX += deltaX *m;
                        fk.positionY += deltaY *m;
                        localSolution.setFastenerAt(k, fk);

                        QList<Tensor> result = m_solver->calculate( &localSolution );
                        Force maxResultantForce = maxLoad(result);

                        if (bestResultantForce > maxResultantForce) {
                            bestResultantForce = maxResultantForce;
                            deepCopy( &localSolution, &bestSolution );
                            emit betterSolutionFound(bestSolution);
                        }
                    }
                }

                /* 'Snap-Grid' Search */
                /* Design Spaces are often defined with borders points that
                 * have rational values, e.g. their x- and y- values can be
                 * expressed as a number that is a division of two integers.
                 * However, the random & local searches previously done
                 * tend to find solutions with x- and y- as real values.
                 * The issue is quite observable near the Design Spaces borders.
                 * To test solutions on the borders, a 'snap-grid' search is done.
                 * It's just a simple test, that changes the fastener position
                 * to rounded/truncated x- and y- values with different precision.
                 * Precision is 3 (mm) or 4 (1/10 of mm).
                 */
                for (int precision = 4; precision >= 3; --precision)
                {
                    Splice intSolution;
                    deepCopy( &bestSolution, &intSolution );
                    Fastener fss = intSolution.fastenerAt(k);
                    fss.positionX = Math::Utils::round( fss.positionX.value(), precision ) *m;
                    fss.positionY = Math::Utils::round( fss.positionY.value(), precision ) *m;
                    intSolution.setFastenerAt(k, fss);

                    QList<Tensor> result = m_solver->calculate( &intSolution );
                    Force maxResultantForce = maxLoad(result);

                    if (bestResultantForce > maxResultantForce) {
                        bestResultantForce = maxResultantForce;
                        deepCopy( &intSolution, &bestSolution );
                        emit betterSolutionFound(bestSolution);

                    }
                }

            }
        }
    }

    m_lock.lockForWrite();
    deepCopy( &bestSolution, m_output);
    m_lock.unlock();
    emit completed();
}

/******************************************************************************
 ******************************************************************************/
QPolygonF OptimisationSolver::createCircle(const QPointF pos, const qreal radius) const
{
    QPolygonF polygon;
    const int incr = 10;
    const int total = (int)(360/incr);
    for (int i = 0; i < total; ++i) {
        qreal angle = qDegreesToRadians( (qreal)(incr*i) );
        QPointF p = pos + QPointF(radius * qCos(angle), radius * qSin(angle));
        polygon << p;
    }
    return polygon;
}

/******************************************************************************
 ******************************************************************************/
bool OptimisationSolver::randomizePosition(Splice *splice)
{
    Q_ASSERT(splice);

    QPolygonF area;
    for (int i = 0; i < splice->designSpaceCount(); ++i) {
        DesignSpace ds = splice->designSpaceAt(i);
        area = area.united( ds.polygon );
    }

    QPolygonF remainingArea;
    remainingArea = area;

    int i = splice->fastenerCount();
    while (i>0) {
        i--;

        if (remainingArea.isEmpty()) {
            // qDebug() << "Warning: Random failed. Restart random positioning.";
            i = splice->fastenerCount();
            remainingArea = area;
            continue;
        }

        QRectF boundingRect = remainingArea.boundingRect();
        Fastener f = splice->fastenerAt(i);
        QPointF proposedPoint;

        int maxiter = 100;
        while (maxiter > 0) {
            maxiter--;

            qreal rand_x = Math::Utils::rand(); /* rand() is between 0.0 and 1.0 */
            qreal rand_y = Math::Utils::rand();

            proposedPoint = QPointF(
                        rand_x * boundingRect.width()  + boundingRect.x(),
                        rand_y * boundingRect.height() + boundingRect.y());

            if (remainingArea.containsPoint(proposedPoint, Qt::OddEvenFill /*WindingFill*/)) {
                break;
            }
        }

        if (maxiter <= 0) {

            /* Here, no solution has been found after 100 iterations.
             * Maybe the design space is too small regarding the number of
             * fasteners, or cannot contain all of them.
             * So, the fastener is is assigned to the first corner of the
             * design space.
             */

            if (!remainingArea.isEmpty()) {
                proposedPoint = remainingArea.first();
                // qDebug() << "Warning: Random failed. Will take the first point:" << proposedPoint;

            } else {
                // qDebug() << "Warning: Random failed. No more place for the remaining fastener(s).";
                return false;
            }
        }

        f.positionX = proposedPoint.x() *m;
        f.positionY = proposedPoint.y() *m;

        splice->setFastenerAt(i, f);



        if (m_constraints.testFlag(OptimisationDesignConstraint::MinPitchDistance_4Phi)) {

            /* Remove the area occuped by the fastener's pitch */
            QPolygonF pitchArea;
            pitchArea = createCircle(proposedPoint, 0.020); // 4*4.78 = 20mm

            if (!remainingArea.boundingRect().isEmpty()) {

                /// \todo subtracted() works ONLY when 'remainingArea' has a
                /// non-null area. If 'remainingArea' is a line or a point,
                /// We should write our own algorithm.

                remainingArea = remainingArea.subtracted( pitchArea );

            } else {

                /// \todo reimplement this in order to remove QPainterPath
                /// and QT += gui from *.PRO/*.PRI project files

                QPainterPath r0;
                r0.addPolygon(remainingArea);

                QPolygonF thinRemainingArea;
                QPainterPathStroker stroker;
                stroker.setWidth(0.000001); // 0.001 mm thick !
                QPainterPath s = stroker.createStroke(r0);
                QPainterPath s_simplified =   s.simplified();
                thinRemainingArea = s_simplified.toFillPolygon();

                remainingArea = thinRemainingArea.subtracted( pitchArea );
            }
        }
    }
    return true;
}

