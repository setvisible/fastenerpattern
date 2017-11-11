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

#ifndef WIDGETS_OPTIMISATION_WIDGET_H
#define WIDGETS_OPTIMISATION_WIDGET_H

#include <QtCore/QThread>
#include <QtWidgets/QWidget>

class Splice;
class SpliceCalculator;
class OptimisationSolver;

namespace Ui {
class OptimisationWidget;
}

class OptimisationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptimisationWidget(QWidget *parent = Q_NULLPTR);
    virtual ~OptimisationWidget();

    void setSpliceCalculator(SpliceCalculator *calculator);

    Splice* initialSplice() const { return m_initialSplice; }
    Splice* currentSplice() const { return m_currentSplice; }


Q_SIGNALS:
    void showInitialToggled(bool checked);
    void showSolutionToggled(bool checked);

private Q_SLOTS:
    void solverStarted();
    void solverProcessed(int percent);
    void solverStopped();
    void solverMessageInfo(qint64 timestamp, QString message);
    void solverMessageWarning(qint64 timestamp, QString message);
    void solverMessageFatal(qint64 timestamp, QString message);

    void startOptimisation();
    void stopOptimisation();

    //temp
    void onShowInitialToggled(bool checked);
    void onShowSolutionToggled(bool checked);

private:
    Ui::OptimisationWidget *ui;
    SpliceCalculator *m_calculator;
    Splice* m_initialSplice;
    Splice* m_currentSplice;
    OptimisationSolver *m_solver;
    QThread m_workerThread;

    void appendMessage(int type, qint64 timestamp, const QString &message);
};

#endif // WIDGETS_OPTIMISATION_WIDGET_H
