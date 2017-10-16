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

#include <Core/Solvers/Parameters>
#include <Core/Tensor>
#include <Core/Splice>

class OptimisationSolver
{
public:
    explicit OptimisationSolver();

    void setDesignObjective(SolverParameters solver);

   // void test();
    void run();

   // QList<Tensor> calculate(const Splice *splice);


private:
    SolverParameters m_solver;

};


#endif // CORE_OPTIMISATION_SOLVER_H
