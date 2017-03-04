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

#ifndef CORE_SOLVER_PARAMETERS_H
#define CORE_SOLVER_PARAMETERS_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

enum class SolverParameters {
    NoSolver,
    RigidBodySolverWithIsoBearing,
    RigidBodySolverWithIsoShear,
    FiniteElementSolver
};

Q_DECLARE_METATYPE(SolverParameters)

QString toString(const SolverParameters param);

#ifdef QT_DEBUG
QDebug operator<<(QDebug dbg, const SolverParameters &param);
#endif

#endif // CORE_SOLVER_PARAMETERS_H
