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

#ifndef CORE_OPTIMISATION_SOLVER_H
#define CORE_OPTIMISATION_SOLVER_H

#include <Core/Solvers/ISolver>
#include <Core/Solvers/Parameters>
#include <Core/Tensor>
#include <Core/Splice>

#include <QtCore/QObject>
//#include <QtCore/QFlags>


class OptimisationSolver : public QObject
{
    Q_OBJECT
public:
    enum DesignObjective {
        MinimizeMaxLoad
        // , MaximizeMinLoad // LOL
    };

    enum DesignConstraint {
        NoConstraint           = 0x0000,
        MinPitchDistance_4Phi  = 0x0001
    };
    //Q_DECLARE_FLAGS(DesignConstraints, DesignConstraint)

    struct RandomHeuristic
    {
        explicit RandomHeuristic(int randomIter = 10, int localIter = 10)
            : randomIterations(randomIter)
            , localIterations(localIter)
        {}
        int randomIterations;
        int localIterations;
    };

public:
    explicit OptimisationSolver(QObject *parent = Q_NULLPTR);

    void setSolver(ISolver *solver);
    void setDesignObjective(DesignObjective objective);
    void setDesignConstraints(DesignConstraint constraints);
    void setDesignOption(RandomHeuristic heuristic);
    void setInput(Splice *input);
    void setOutput(Splice *output);

    void start();
    void stop();


Q_SIGNALS:
    void started();
    void processed(int percent);
    void stopped();

    /* Logs */
    void messageInfo(qint64 timestamp, QString message);
    void messageWarning(qint64 timestamp, QString message);
    void messageFatal(qint64 timestamp, QString message);

public Q_SLOTS:

private:
    ISolver *m_solver;
    Splice *m_input;
    Splice *m_output;
    DesignObjective m_objective;
    DesignConstraint m_constraints;
    RandomHeuristic m_heuristic;
    bool m_isRunning;

    bool randomizePosition(Splice *splice);
    void sendMessage(const Force bestResultantForce, const Splice &bestSolution);


    bool sanitaryChecks();
    qint64 timestamp() const;
    QPolygonF createCircle(const QPointF pos, const qreal radius) const;

    Force maxLoad(const QList<Tensor> result) const;
};

//Q_DECLARE_OPERATORS_FOR_FLAGS(OptimisationSolver::DesignConstraints)


#endif // CORE_OPTIMISATION_SOLVER_H
