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

#include "parameters.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

QString toString(const SolverParameters param)
{
    QString str;
    switch (param) {
    case SolverParameters::NoSolver:
        str = QStringLiteral("No Solver");  break;
    case SolverParameters::RigidBodySolverWithIsoBearing:
        str = QStringLiteral("Rigid Body Solver (IsoBearing)");  break;
    case SolverParameters::RigidBodySolverWithIsoShear:
        str = QStringLiteral("Rigid Body Solver (IsoShear)");  break;
    case SolverParameters::FiniteElementSolver:
        str = QStringLiteral("Finite Element Solver");  break;
    default:
        // Q_UNREACHABLE;
        break;
    }
    return str;
}


#ifdef QT_DEBUG
QDebug operator<<(QDebug dbg, const SolverParameters &param)
{
    dbg.nospace() << toString(param);
    return dbg.maybeSpace();
}
#endif

