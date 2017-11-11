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
#include <Maths/Utils>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtGui/QPolygonF>

// #include <map>
// #include <string>
// #include <list>
// #include <vector>

#include <QPainterPath>
#include <QDateTime>
#include <QtCore/QtMath>


OptimisationSolver::OptimisationSolver(QObject *parent) : QObject(parent)
  , m_solver(Q_NULLPTR)
  , m_input(Q_NULLPTR)
  , m_output(Q_NULLPTR)
  , m_objective(DesignObjective::MinimizeMaxLoad)
  , m_constraints(DesignConstraint::NoConstraint)
  , m_heuristic(RandomHeuristic())
  , m_isRunning(false)
{

}

/******************************************************************************
 ******************************************************************************/
void OptimisationSolver::setSolver(ISolver *solver)
{
    m_solver = solver;
}

void OptimisationSolver::setDesignObjective(DesignObjective objective)
{
    m_objective = objective;
}

void OptimisationSolver::setDesignConstraints(DesignConstraint constraints)
{
    m_constraints = constraints;
}

// max iteration...
void OptimisationSolver::setDesignOption(RandomHeuristic heuristic)
{
    m_heuristic = heuristic;
}

void OptimisationSolver::setInput(Splice *input)
{
    m_input = input;
}

void OptimisationSolver::setOutput(Splice *output)
{
    /* The reason why we use a pointer to Splice is
     * to avoid multiple message sending containing Splice objects.
     *
     * The normal way would be to emit messages with Splice, e.g. found(Splice obj).
     * But a splice object contains big data.
     *
     * That is, the rest of the program is able to show the current
     * best solution and display a real time picture of it.
     */
    m_output = output;
}



/******************************************************************************
 ******************************************************************************/
static inline void designSpacesCopy(const Splice *from, Splice *to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);
    to->removeAllDesignSpaces();
    for (int i = 0; i < from->designSpaceCount(); ++i) {
        to->addDesignSpace(from->designSpaceAt(i));
    }

}
static inline void fastenersCopy(const Splice *from, Splice *to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);
    to->removeAllFasteners();
    for (int i = 0; i < from->fastenerCount(); ++i) {
        to->addFastener(from->fastenerAt(i));
    }

}

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
Force OptimisationSolver::maxLoad(const QList<Tensor> result) const
{
    Force maxResultantForce = 0.0 *N;
    for (int j = 0; j < result.count(); ++j) {
        const Tensor f = result.at(j);
        const Force resultant = f.resultantFxy();
        if (maxResultantForce < resultant) {
            maxResultantForce = resultant;
        }
    }
    return maxResultantForce;
}

/******************************************************************************
 ******************************************************************************/
void OptimisationSolver::sendMessage(const Force bestResultantForce,
                                     const Splice &bestSolution)
{
    auto ts = timestamp();
    QString message;
    message = QString("MaxLoad = %0 N, with:").arg(bestResultantForce.value());
    emit messageInfo(ts, message);

    for (int k = 0; k < bestSolution.fastenerCount(); ++k) {
        QString details;
        details = QString("  - fastener %0 at (%1,%2)")
                .arg(k)
                .arg(bestSolution.fastenerAt(k).positionX.value())
                .arg(bestSolution.fastenerAt(k).positionY.value());

        emit messageInfo(ts, details);
    }
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Return a timestamp information with microsecond precision.
 */
qint64 OptimisationSolver::timestamp() const
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

/******************************************************************************
 ******************************************************************************/
bool OptimisationSolver::sanitaryChecks()
{
    bool isValid = true;
    if (!m_solver) {
        emit messageFatal(timestamp(), QLatin1String("Error: the optimizer requires a solver."));
        isValid = false;
    }
    if (!m_input) {
        emit messageFatal(timestamp(), QLatin1String("Error: the optimizer requires an input splice."));
        isValid = false;
    }
    if (!m_output) {
        emit messageFatal(timestamp(), QLatin1String("Error: the optimizer requires an output splice."));
        isValid = false;
    }
    if (m_input) {
        if (m_input->appliedLoad() == Tensor()) {
            emit messageFatal( timestamp(), QLatin1String("Error: the splice must have a non-zero applied load."));
            isValid = false;
        }
        if (m_input->designSpaceCount() == 0) {
            emit messageFatal(timestamp(), QLatin1String("Error: the splice must have a design space."));
            isValid = false;
        }
        if (m_input->fastenerCount() == 0) {
            emit messageFatal(timestamp(), QLatin1String("Error: the splice must have a fastener."));
            isValid = false;
        }
    }
    return isValid;
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Start the solution exploration.
 *
 * \remark This is a blocking method.
 * Since it's a expensive task, it's strongly recommanded
 * to start this method from another thread, e.g. a QThread worker thread.
 */
void OptimisationSolver::start()
{
    emit started();
    emit messageInfo(timestamp(), QLatin1String("Started."));
    m_isRunning = true;

    if (!sanitaryChecks()) {
        emit messageInfo(timestamp(), QLatin1String("Failed."));
        emit stopped();
        m_isRunning = false;
        return;
    }

    Q_ASSERT(m_solver);
    Q_ASSERT(m_input);
    Q_ASSERT(m_output);

    QPolygonF area;
    for (int i = 0; i < m_input->designSpaceCount(); ++i) {
        DesignSpace ds = m_input->designSpaceAt(i);
        area = area.united( ds.polygon );
    }

    Splice bestSolution;

    Force bestResultantForce = 1000000.0 * m_input->appliedLoad().resultantFxy();
    Q_ASSERT(bestResultantForce.value() > 0.0);

    // 1st iteration loop
    for (int i = 0; i < m_heuristic.randomIterations; ++i) {

        if (!m_isRunning)
            break;

        Splice solution;
        deepCopy( m_input, &solution );

        bool ok = randomizePosition( &solution );
        if (!ok) {
            continue;
        }

        {
            QList<Tensor> result = m_solver->calculate( &solution );
            Force maxResultantForce = maxLoad(result);
            if (bestResultantForce > maxResultantForce) {
                bestResultantForce = maxResultantForce;
                deepCopy( &solution, &bestSolution );
            }
        }

        // **********************
        for (int j = 0; j < m_heuristic.localIterations; ++j) {

            if (!m_isRunning)
                break;

            qreal delta = 1.0 / (qreal)(j+1);

            //   qDebug() << "  Local Loop" << j+1 << "/" << m_heuristic.localIterations;

            // localOptimisation(  &solution );
            // on deplace chaque F de 1mm dans toutes les directions pour trouver son min local
            for (int k = 0; k < solution.fastenerCount(); ++k) {

                /*
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

                int size = 2;
                Q_ASSERT(size>0);
                for (int ii = -size; ii < size; ++ii) {
                    for (int jj = -size; jj < size; ++jj) {

                        qreal deltaX = ii * delta;
                        qreal deltaY = jj * delta;


                        Splice localSolution;
                        deepCopy( &solution, &localSolution );

                        Fastener fk = localSolution.fastenerAt(k);

                        QPointF proposedPoint;
                        proposedPoint = QPointF(
                                    fk.positionX.value() + deltaX,
                                    fk.positionY.value() + deltaY);

                        if (! area.containsPoint(proposedPoint, Qt::WindingFill)) {
                            continue;
                        }
                        // else

                        fk.positionX += deltaX *m;
                        fk.positionY += deltaY *m;
                        localSolution.setFastenerAt(k, fk);

                        QList<Tensor> result = m_solver->calculate( &localSolution );
                        Force maxResultantForce = maxLoad(result);

                        if (bestResultantForce > maxResultantForce) {
                            bestResultantForce = maxResultantForce;
                            deepCopy( &localSolution, &bestSolution );
                            sendMessage(bestResultantForce, bestSolution);
                        }
                    }
                }

                // on essaye aussi la solution entiere la plus proche
                for (int precision = 3; precision >= 0; --precision)
                {

                    Splice intSolution;
                    deepCopy( &bestSolution, &intSolution );
                    Fastener fss = intSolution.fastenerAt(k);
                    fss.positionX = Maths::Utils::round( fss.positionX.value(), precision ) *m;
                    fss.positionY = Maths::Utils::round( fss.positionY.value(), precision ) *m;
                    intSolution.setFastenerAt(k, fss);

                    QList<Tensor> result = m_solver->calculate( &intSolution );
                    Force maxResultantForce = maxLoad(result);

                    if (bestResultantForce > maxResultantForce) {
                        bestResultantForce = maxResultantForce;
                        deepCopy( &intSolution, &bestSolution );
                        sendMessage(bestResultantForce, bestSolution);

                    }
                }

            }
        }
    }

    emit messageInfo(timestamp(), QLatin1String("Final Solution:"));
    sendMessage(bestResultantForce, bestSolution);

    deepCopy( &bestSolution, m_output);

    emit messageInfo(timestamp(), QLatin1String("Finished."));
    emit stopped();
}

/******************************************************************************
 ******************************************************************************/
void OptimisationSolver::stop()
{
    emit messageWarning(timestamp(), QLatin1String("Stopping..."));
    m_isRunning = false;
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
            qDebug() << "Area is empty! restart!   i was" << i;
            // restart
            i = splice->fastenerCount() - 1;
            remainingArea = area;
        }
        QRectF boundingRect = remainingArea.boundingRect();


        Fastener f = splice->fastenerAt(i);

        QPointF proposedPoint;
        int maxiter = 100;
        while (maxiter > 0) {
            maxiter--;

            qreal rand_x = Maths::Utils::rand(); // between 0 and 1
            qreal rand_y = Maths::Utils::rand();

            proposedPoint = QPointF(
                        rand_x * boundingRect.width()  + boundingRect.x(),
                        rand_y * boundingRect.height() + boundingRect.y());

            if (remainingArea.containsPoint(proposedPoint, Qt::WindingFill)) {
                break;
            }
        }

        if (maxiter <= 0) {

            // Au pire, on place le fast dans le 1er coin de la remaining area

            if (!remainingArea.isEmpty()) {
                proposedPoint = remainingArea.first();

            } else {
                qDebug() << Q_FUNC_INFO << "Warning: Possible point not found.";
                return false;
            }
        }

        f.positionX = proposedPoint.x() *m;
        f.positionY = proposedPoint.y() *m;

        splice->setFastenerAt(i, f);

        // remove the area occuped bu the pitch !
        QPolygonF pitchArea;
        pitchArea = createCircle(proposedPoint, 0.006);

        if (!remainingArea.boundingRect().isEmpty()) {
            // subtracted() ne fonctionne QUE quand remaining area a une surface non nulle
            // Pour les lignes / poins, il faut ecrire un algo maison
            remainingArea = remainingArea.subtracted( pitchArea );

        } else {

            /// \todo reimplement this in order to remove QPainterPath
            /// and QT += gui from PRO
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

    return true;
}

