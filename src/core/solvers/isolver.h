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

#ifndef CORE_SOLVERS_ISOLVER_H
#define CORE_SOLVERS_ISOLVER_H

#include <Core/Tensor>
#include <Core/Splice>

#include <QtCore/QObject>

/*! \class ISolver
 *  \brief The class ISolver is an interface for the solvers.
 *
 * A solver calculates the distribution of loads for a given fastener pattern, using a specific method.
 *
 * For instance, we can implement these solvers:
 * \li RigidBodySolver         A solver that uses the Rigid Bodies Method.
 * \li StaticLinearFEMSolver   A solver that uses a Finite Element Method (i.e. static analysis).
 * \li NonLinearFEMSolver      A solver that uses a Finite Element Method with Non-Linear Approximation.
 * \li OptimisationSolver      A solver that calculates the optimal pattern, with Gecode or Google OR-tools.
 * \li etc.
 *
 */
class ISolver : public QObject
{
public:
    explicit ISolver(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    virtual ~ISolver() {}

    virtual QList<Tensor> calculate(const Splice *splice) = 0;

};

#endif // CORE_SOLVERS_ISOLVER_H
