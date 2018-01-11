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

#include <Core/AbstractSpliceModel>
#include <Core/Splice>
#include <Core/Optimizer/Controller>
#include <Core/Optimizer/OptimisationSolver>

#include <boost/units/cmath.hpp> /* pow<>() */

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

/*! \class OptimisationWidget
 * \brief The class OptimisationWidget is the GUI that allows User
 * to start, follow and stop the optimisation analysis.
 *
 * \remark The OptimisationSolver::start() method is an expensive task.
 * Then, the Controller runs another thread than the GUI.
 *
 * \remark Due to the limited width of the OptimisationWidget,
 * try to keep the length of the messages less than 40 characters.
 */
OptimisationWidget::OptimisationWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::OptimisationWidget)
  , m_controller(new Controller(this))
  , m_calculator(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->console->clear();
    ui->detailOutput->clear();

    ui->console->setWordWrapMode(QTextOption::NoWrap);
    ui->console->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->console->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->detailOutput->setWordWrapMode(QTextOption::NoWrap);
    ui->detailOutput->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->detailOutput->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    const int maxlines = 5;
    const QFontMetrics fm = ui->detailOutput->fontMetrics();
    ui->detailOutput->setMinimumHeight( (fm.lineSpacing()+2) * maxlines + 10);
    ui->detailOutput->setMaximumHeight( (fm.lineSpacing()+2) * maxlines + 10);
    ui->detailOutput->setMaximumBlockCount( maxlines );


    /* Connect the Controller */
    connect(ui->startButton, SIGNAL(released()), this, SLOT(start()));
    connect(ui->stopButton, SIGNAL(released()), this, SLOT(stop()));

    connect(m_controller, SIGNAL(started()), this, SLOT(onControllerStarted()));
    connect(m_controller, SIGNAL(progressed(int)), this, SLOT(onControllerProgressed(int)));
    connect(m_controller, SIGNAL(stopped()), this, SLOT(onControllerStopped()));

    connect(m_controller, SIGNAL(messageInfo(qint64,QString)),
            this, SLOT(onControllerMessageInfo(qint64,QString)));

    connect(m_controller, SIGNAL(messageWarning(qint64,QString)),
            this, SLOT(onControllerMessageWarning(qint64,QString)));

    connect(m_controller, SIGNAL(messageFatal(qint64,QString)),
            this, SLOT(onControllerMessageFatal(qint64,QString)));

    connect(m_controller, SIGNAL(messageDebug(QString)),
            this, SLOT(onControllerMessageDebug(QString)));

    /* Connect the GUI */
    connect(ui->showResultCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(onShowResultToggled(bool)));

    /* Reset the GUI */
    onControllerStopped();
}

OptimisationWidget::~OptimisationWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
AbstractSpliceModel *OptimisationWidget::model() const
{
    return m_calculator;
}

void OptimisationWidget::setModel(AbstractSpliceModel *calculator)
{
    m_calculator = calculator;
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::onShowResultToggled(bool checked)
{
    Splice *splice = (checked ? m_controller->output()
                              : m_controller->input() );
    for (int i = 0; i < splice->fastenerCount(); ++i) {
        m_calculator->setFastener(i, splice->fastenerAt(i));
    }
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::onControllerStarted()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->showTimestampCheckBox->setEnabled(false);
    ui->progressBar->setVisible(true);
}

void OptimisationWidget::onControllerProgressed(int percent)
{
    ui->progressBar->setValue(percent);
}

void OptimisationWidget::onControllerStopped()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->showTimestampCheckBox->setEnabled(true);
    ui->progressBar->setVisible(false);
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::onControllerMessageInfo(qint64 timestamp, QString message)
{
    log(0, timestamp, message);
}

void OptimisationWidget::onControllerMessageWarning(qint64 timestamp, QString message)
{
    log(1, timestamp, message);
}

void OptimisationWidget::onControllerMessageFatal(qint64 timestamp, QString message)
{
    log(2, timestamp, message);
}

void OptimisationWidget::onControllerMessageDebug(QString message)
{
    ui->detailOutput->appendPlainText(message);
}

/******************************************************************************
 ******************************************************************************/
void OptimisationWidget::log(int type, qint64 timestamp, const QString &message)
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
void OptimisationWidget::start()
{
    /// \todo Save the initial splice somewhere

    /// \bug memory leak !
    Splice* input = new Splice();

    input->setAppliedLoad( m_calculator->appliedLoad() );
    for (int i = 0; i < m_calculator->designSpaceCount(); ++i) {
        input->addDesignSpace( m_calculator->designSpaceAt(i) );
    }
    for (int i = 0; i < m_calculator->fastenerCount(); ++i) {
        input->addFastener( m_calculator->fastenerAt(i) );
    }

    m_controller->setSolver( m_calculator->solver() );
    m_controller->setInput(input);
    m_controller->start();
}

void OptimisationWidget::stop()
{
    m_controller->cancel();
}
