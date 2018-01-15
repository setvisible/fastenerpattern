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

#include "controller.h"

#include "maxminload.h"
#include <Core/Optimizer/OptimisationSolver>
#include <Core/Solvers/ISolver>
#include <Core/Splice>

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QRunnable>
#include <QtCore/QThreadPool>
#include <QtCore/QtMath> /* qFloor() */

/******************************************************************************
 ******************************************************************************/

/*! \class Task
 * \remark QRunnable cannot emit Signals. It's not a QObject.
 *         The workaround is to emit Signals from a OptimisationSolver.
 */
class Task : public QRunnable
{
public:
    Task(OptimisationSolver *s) : QRunnable(), m_optimizer(s) {}

public:
    void run() Q_DECL_OVERRIDE
    {
        m_optimizer->runAsync();
    }

private:
    /// \todo Use a functor *f() instead ?
    OptimisationSolver* m_optimizer;
};


/******************************************************************************
 ******************************************************************************/

/*! \class Controller
 * \brief The class Controller manages the optimisation from A to Z.
 * In particular, Controller manages the creation and destruction of the threads
 * during the computation, in order to balance the computation load
 * on multiple CPUs.
 *
 * Note that OptimisationSolver::runAsync() performs one unique optimisation loop.
 *
 * Controller launches OptimisationSolver::runAsync() one after one,
 * and emits signals to be eventually catched by the GUI.
 *
 * \remark OptimisationSolver MUST be thread-safe (and then reentrant).
 *
 * \sa OptimisationSolver
 */
Controller::Controller(QObject *parent) : QObject(parent)
  , m_optimizer(new OptimisationSolver(this))
  , m_solver(Q_NULLPTR)
  , m_input(QSharedPointer<Splice>(new Splice))
  , m_output(QSharedPointer<Splice>(new Splice))
  , m_iteration(0)
  , m_iterationCount(10000)
{

    /* When a task is completed, the controller starts a new task directly,
     * without waiting for the end of the other running tasks.
     * It's possible to run a slot immediately in the Qt's events queue
     * with 'Qt::DirectConnection'.
     */
    connect(m_optimizer, SIGNAL(completed()),
            this, SLOT(onTaskCompleted()), Qt::DirectConnection);

    connect(m_optimizer, SIGNAL(errorDetected(OptimisationErrorType)),
            this, SLOT(onErrorDetected(OptimisationErrorType)));

    connect(m_optimizer, SIGNAL(betterSolutionFound(Splice)),
            this, SLOT(onBetterSolutionFound(Splice)), Qt::BlockingQueuedConnection);

}

Controller::~Controller()
{
    /// \todo wait for the remaining tasks to finished ?
    waitForFinishing();
}

/******************************************************************************
 ******************************************************************************/
ISolver *Controller::solver() const
{
    return m_solver;
}

void Controller::setSolver(ISolver *solver)
{
    m_solver = solver;
}

QSharedPointer<Splice> Controller::input() const
{
    return m_input;
}

void Controller::setInput(const QSharedPointer<Splice> &splice)
{
    m_input = splice;
}

QSharedPointer<Splice> Controller::output() const
{
    return m_output;
}

/******************************************************************************
 ******************************************************************************/
void Controller::onTaskCompleted()
{
    if (m_iteration > 0) {
        m_iteration--;
        runTask();
    } else if (m_iteration == 0) {
        waitForFinishing();
        emit progressed(100);
        emit messageInfo(timestamp(), tr("Finished."));
        emit stopped();
    }
}

void Controller::onErrorDetected(OptimisationErrorType error)
{
    emit messageFatal(timestamp(), toString(error));
}

void Controller::onBetterSolutionFound(const Splice &solution)
{
    QList<Tensor> result = m_solver->calculate( &solution );
    Force bestResultantForce = maxLoad(result);

    auto ts = timestamp();
    QString message;
    message = QString("MaxLoad = %0 N, with:").arg(bestResultantForce.value());
    emit messageInfo(ts, message);

    for (int k = 0; k < solution.fastenerCount(); ++k) {
        QString details;
        details = QString("  - fastener %0 at (%1,%2)")
                .arg(k)
                .arg(solution.fastenerAt(k).positionX.value())
                .arg(solution.fastenerAt(k).positionY.value());
        emit messageInfo(ts, details);
    }
}

/******************************************************************************
 ******************************************************************************/
void Controller::runTask()
{
    QThreadPool::globalInstance()->start( new Task(m_optimizer) );
    if (m_iterationCount != 0) {
        const qreal percent = 1 - ((qreal)m_iteration / (qreal)m_iterationCount);
        emit progressed(qFloor(100.0*percent));
    }
}

/******************************************************************************
 ******************************************************************************/
void Controller::cancel()
{
    emit messageInfo(timestamp(), tr("Cancelling..."));
    waitForFinishing();
    emit progressed(0);
    emit messageInfo(timestamp(), tr("Cancelled."));
    emit stopped();
}

/******************************************************************************
 ******************************************************************************/
void Controller::waitForFinishing()
{
    m_iteration = -1;

    /* We wait. Hope the running tasks finish soon. */
    /* remark: waitForDone() blocks all threads even GUI! */
    QThreadPool::globalInstance()->waitForDone(100); /* 100 milliseconds */

    m_optimizer->postcompute();
}

/******************************************************************************
 ******************************************************************************/
void Controller::start()
{
    /*********************************************************************\
    * - prepare the optimizer                                             *
    * - do Sanitary Checks                                                *
    * - pre-calculation                                                   *
    * - launch the tasks                                                  *
    * - ...                                                               *
    * - post-calculation                                                  *
    \*********************************************************************/
    Q_ASSERT(m_optimizer);
    Q_ASSERT(m_solver);
    Q_ASSERT(!m_input.isNull());
    Q_ASSERT(!m_output.isNull());

    emit started();
    emit progressed(0);
    emit messageInfo(timestamp(), tr("Started."));
    emit messageDebug(tr("..."));


    m_optimizer->setSolver( m_solver ); /// \todo Functor instead ?
    m_optimizer->setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    m_optimizer->setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    m_optimizer->setRandomIterations( 1 );
    m_optimizer->setInput( m_input.data() );
    m_optimizer->setOutput( m_output.data() );

    m_iteration = m_iterationCount;

    if (!m_optimizer->sanitarize()) {
        emit messageInfo(timestamp(), tr("Failed."));
        emit stopped();
        return;
    }

    m_optimizer->precompute();

    /* Remark:
     * 'idealThreadCount' is a hack to increase the core load.
     * On my octo-core machine, 'idealThreadCount()' is 10 by default.
     * Curiously, the cores seem to be under-loaded (never more than 50%).
     * However, by increasing manually 'maxThreadCount' to 20~30,
     * the load reaches 100%.
     */
    const int idealThreadCount = 30; // Magic Number
    const int threadCount = qMin(m_iteration, idealThreadCount);
    QThreadPool::globalInstance()->setMaxThreadCount(threadCount);

    qDebug() << Q_FUNC_INFO;
    qDebug() << "activeThreadCount" << QThreadPool::globalInstance()->activeThreadCount();
    qDebug() << "maxThreadCount"    << QThreadPool::globalInstance()->maxThreadCount();
    qDebug() << "idealThreadCount"  << QThread::idealThreadCount();

    /* Warning!
     * Keep 'idealThreadCount' as low as possible.
     * This loop blocks all the threads even GUI!
     */
    for (int i = 0; i < threadCount; ++i) {
        this->runTask();
    }

}

/******************************************************************************
 ******************************************************************************/
inline QString Controller::toString(OptimisationErrorType error) const
{
    switch (error) {
    case OptimisationErrorType::ERR_UNDEFINED_SOLVER:
        return tr("Error: the optimizer requires a solver.");
        break;
    case OptimisationErrorType::ERR_UNDEFINED_INPUT_SPLICE:
        return tr("Error: the optimizer requires an input splice.");
        break;
    case OptimisationErrorType::ERR_UNDEFINED_OUTPUT_SPLICE:
        return tr("Error: the optimizer requires an output splice.");
        break;
    case OptimisationErrorType::ERR_NO_APPLIED_LOAD:
        return tr("Error: the splice must have a non-zero applied load.");
        break;
    case OptimisationErrorType::ERR_NO_DESIGNSPACE:
        return tr("Error: the splice must have a design space.");
        break;
    case OptimisationErrorType::ERR_NO_FASTENER:
        return tr("Error: the splice must have a fastener.");
        break;
    default:
        Q_UNREACHABLE();
        break;
    }
}

/*! \brief Return a timestamp information with microsecond precision.
 */
inline qint64 Controller::timestamp() const
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

