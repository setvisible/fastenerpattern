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

#ifndef CORE_RIGID_BODY_SOLVER_H
#define CORE_RIGID_BODY_SOLVER_H

#include <Core/Solvers/ISolver>
#include <Core/Solvers/Parameters>

class QObject;

class RigidBodySolver : public ISolver
{
    Q_PROPERTY(SolverParameters parameters READ parameters WRITE setParameters)

public:
    explicit RigidBodySolver(QObject *parent = Q_NULLPTR);
    ~RigidBodySolver();

    virtual QList<Tensor> calculate(const Splice *splice) Q_DECL_OVERRIDE;

    SolverParameters parameters() const;
    void setParameters(SolverParameters parameters);

private:
    SolverParameters m_params;
};


#endif // CORE_RIGID_BODY_SOLVER_H
