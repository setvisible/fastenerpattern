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

#include "optimisationwidget.h"
#include "ui_optimisationwidget.h"

#include <Core/SpliceCalculator>
#include <Core/Optimizer/OptimisationSolver>
#include <boost/units/cmath.hpp> /* pow<>() */

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

/*! \class OptimisationWidget
 * \brief The class OptimisationWidget is the GUI that start and stops the
 * optimisation analysis.
 *
 * The OptimisationSolver::start() method is an expensive task.
 * This is why it's run by another thread than the GUI.
 *
 * The OptimisationWidget controls the OptimisationSolver's thread.
 */
OptimisationWidget::OptimisationWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::OptimisationWidget)
  , m_calculator(Q_NULLPTR)
  , m_initialSplice(new Splice(this))
  , m_currentSplice(new Splice(this))
  , m_solver(Q_NULLPTR)
{
    ui->setupUi(this);

    this->solverStopped();

    /* Create the solver and attach it to a separated thread worker */
    m_solver = new OptimisationSolver(); /* rem: Must have no parent! */
    m_solver->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_solver, &QObject::deleteLater);

    connect(m_solver, SIGNAL(started()), this, SLOT(solverStarted()));
    connect(m_solver, SIGNAL(stopped()), this, SLOT(solverStopped()));
    connect(m_solver, SIGNAL(processed(int)), this, SLOT(solverProcessed(int)));
    connect(m_solver, SIGNAL(messageInfo(qint64,QString)),
            this, SLOT(solverMessageInfo(qint64,QString)));
    connect(m_solver, SIGNAL(messageWarning(qint64,QString)),
            this, SLOT(solverMessageWarning(qint64,QString)));
    connect(m_solver, SIGNAL(messageFatal(qint64,QString)),
            this, SLOT(solverMessageFatal(qint64,QString)));

    //  connect(ui->stopButton, SIGNAL(released()), m_solver, SLOT(stop());

    // connect(m_solver, &OptimisationSolver::resultReady, this, &OptimisationWidget::handleResults);

    m_workerThread.start();


    /* Connect the GUI */
    connect(ui->startButton, SIGNAL(released()), this, SLOT(startOptimisation()));
    connect(ui->stopButton, SIGNAL(released()), this, SLOT(stopOptimisation()));
    /// \todo connect(ui->showInitialCheckBox, SIGNAL(toggled(bool)), SIGNAL(showInitialToggled(bool)));
    /// \todo connect(ui->showSolutionCheckBox, SIGNAL(toggled(bool)), SIGNAL(showSolutionToggled(bool)));
    connect(ui->showInitialCheckBox, SIGNAL(toggled(bool)),  this, SLOT(onShowInitialToggled(bool)));
    connect(ui->showSolutionCheckBox, SIGNAL(toggled(bool)), this, SLOT(onShowSolutionToggled(bool)));

    ui->console->setWordWrapMode(QTextOption::NoWrap);
    ui->console->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->console->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

OptimisationWidget::~OptimisationWidget()
{
    m_workerThread.quit();
    m_workerThread.wait();
    delete ui;
}


/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::setSpliceCalculator(SpliceCalculator *calculator)
{
    m_calculator = calculator;
}

/* TEMPO */
void OptimisationWidget::onShowInitialToggled(bool checked)
{

}

void OptimisationWidget::onShowSolutionToggled(bool checked)
{
    Splice *splice = (!checked ? m_initialSplice : m_currentSplice );
    for (int i = 0; i < splice->fastenerCount(); ++i) {
        m_calculator->setFastener(i, splice->fastenerAt(i));
    }
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::solverStarted()
{
    qDebug() << Q_FUNC_INFO;
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->showTimestampCheckBox->setEnabled(false);
    repaint();
}

void OptimisationWidget::solverProcessed(int /*percent*/)
{
    qDebug() << Q_FUNC_INFO;
    /// \todo
}

void OptimisationWidget::solverStopped()
{
    qDebug() << Q_FUNC_INFO;
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->showTimestampCheckBox->setEnabled(true);
}

void OptimisationWidget::solverMessageInfo(qint64 timestamp, QString message)
{
    this->appendMessage(0, timestamp, message);
}

void OptimisationWidget::solverMessageWarning(qint64 timestamp, QString message)
{
    this->appendMessage(1, timestamp, message);
}

void OptimisationWidget::solverMessageFatal(qint64 timestamp, QString message)
{
    this->appendMessage(2, timestamp, message);
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::appendMessage(int type, qint64 timestamp, const QString &message)
{
    switch (type) {
    case 0: ui->console->setTextColor(Qt::black); break;
    case 1: ui->console->setTextColor(Qt::blue); break;
    case 2: ui->console->setTextColor(Qt::red); break;
    default:
        Q_UNREACHABLE();
        break;
    }

    if (ui->showTimestampCheckBox->isChecked()) {
        QDateTime datetime = QDateTime::fromMSecsSinceEpoch(timestamp);
        QString str = datetime.toString("yyyy-mm-dd_hh:mm:ss.zzz");
        QString text = QString("[%0] %1").arg(str).arg(message);
        ui->console->append(text);
    } else {
        ui->console->append(message);
    }
}


/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::startOptimisation()
{
    Q_ASSERT(m_calculator);

    qDebug() << Q_FUNC_INFO;

    /* Save the initial splice */
    m_initialSplice->removeAllFasteners();
    m_initialSplice->removeAllDesignSpaces();

    m_initialSplice->setTitle( QString() );
    m_initialSplice->setAuthor(QString());
    m_initialSplice->setDate(QString());
    m_initialSplice->setDescription(QString());
    m_initialSplice->setAppliedLoad( m_calculator->appliedLoad() );
    for (int i = 0; i < m_calculator->designSpaceCount(); ++i) {
        m_initialSplice->addDesignSpace( m_calculator->designSpaceAt(i) );
    }
    for (int i = 0; i < m_calculator->fastenerCount(); ++i) {
        m_initialSplice->addFastener( m_calculator->fastenerAt(i) );
    }

    /* Prepare the solver */
    ISolver *solver = m_calculator->solver();
    m_solver->setSolver( solver );
    m_solver->setDesignObjective( OptimisationSolver::MinimizeMaxLoad );
    m_solver->setDesignConstraints( OptimisationSolver::MinPitchDistance_4Phi );
    m_solver->setDesignOption( OptimisationSolver::RandomHeuristic(100,10) );
    m_solver->setInput( m_initialSplice );
    m_solver->setOutput( m_currentSplice );

    m_solver->start();
}

void OptimisationWidget::stopOptimisation()
{
    m_solver->stop();
}
