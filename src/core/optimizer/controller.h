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

#ifndef CORE_OPTIMISATION_CONTROLLER_H
#define CORE_OPTIMISATION_CONTROLLER_H

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

class ISolver;
class Splice;
class OptimisationSolver;
enum class OptimisationErrorType;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = Q_NULLPTR);
    ~Controller();

    Splice *input() const;
    void setInput(Splice *splice);

    ISolver *solver() const;
    void setSolver(ISolver *solver);

    Splice *output() const;

    void start();
    void cancel();

public Q_SLOTS:
    void onTaskCompleted();
    void onErrorDetected(OptimisationErrorType error);
    void onBetterSolutionFound(const Splice &solution);

Q_SIGNALS:
    void started();
    void progressed(int percent); /* between 0 and 100 */
    void stopped();

    void messageInfo(qint64 timestamp, QString message);
    void messageWarning(qint64 timestamp, QString message);
    void messageFatal(qint64 timestamp, QString message);
    void messageDebug(QString message);

private:
    OptimisationSolver *m_optimizer;
    ISolver *m_solver;
    Splice* m_input;
    Splice* m_output;
    int m_iteration;
    int m_iterationCount;

    void runTask();
    void waitForFinishing();

    inline QString toString(OptimisationErrorType error) const;
    inline qint64 timestamp() const;
};

#endif // CORE_OPTIMISATION_CONTROLLER_H
