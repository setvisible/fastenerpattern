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

#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtGui/QPolygonF>

QT_BEGIN_NAMESPACE
class QPointF;
QT_END_NAMESPACE

class ISolver;
class Splice;

enum class OptimisationErrorType {
    ERR_UNDEFINED_SOLVER,
    ERR_UNDEFINED_INPUT_SPLICE,
    ERR_UNDEFINED_OUTPUT_SPLICE,
    ERR_NO_APPLIED_LOAD,
    ERR_NO_DESIGNSPACE,
    ERR_NO_FASTENER
};

enum class OptimisationDesignObjective {
    MinimizeMaxLoad,
    MaximizeMinLoad
};

enum class OptimisationDesignConstraint {
    NoConstraint           = 0x0000,
    MinPitchDistance_4Phi  = 0x0001
};

/// \todo Q_DECLARE_FLAGS(OptimisationDesignConstraints, OptimisationDesignConstraint)
/// \todo Q_DECLARE_OPERATORS_FOR_FLAGS(OptimisationDesignConstraints)


class OptimisationSolver : public QObject
{
    Q_OBJECT
public:
    explicit OptimisationSolver(QObject *parent = Q_NULLPTR);
    ~OptimisationSolver();

    ISolver *solver() const;
    void setSolver(ISolver *solver);

    Splice *input() const;
    void setInput(Splice *input);

    Splice *output() const;
    void setOutput(Splice *output);

    int randomIterations() const;
    void setRandomIterations(const int iterations);

    OptimisationDesignObjective objective() const;
    void setDesignObjective(OptimisationDesignObjective objective);

    /// \todo OptimisationDesignConstraint__*s*
    OptimisationDesignConstraint constraints() const;
    void setDesignConstraints(OptimisationDesignConstraint constraints);

    void runSync();
    void runAsync();

Q_SIGNALS:
    void errorDetected(OptimisationErrorType code);
    void finished();
    void betterSolutionFound(const Splice &solution);
    void completed(); /* successfully finished */

protected:
    friend class Controller;
    bool sanitarize();
    void precompute();
    void postcompute();

private:
    QReadWriteLock m_lock;
    ISolver *m_solver;
    Splice *m_input;
    Splice *m_output;
    OptimisationDesignObjective m_objective;
    OptimisationDesignConstraint m_constraints;
    int m_randomIterations;
    int m_localIterations;

    QPolygonF m_precomputedArea;

    bool randomizePosition(Splice *splice);
    QPolygonF createCircle(const QPointF pos, const qreal radius) const;

};

#endif // CORE_OPTIMISATION_SOLVER_H
