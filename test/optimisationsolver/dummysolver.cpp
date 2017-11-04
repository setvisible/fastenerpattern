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

#include "dummysolver.h"

#include <boost/units/cmath.hpp> // pow()

/*! \class DummySolver
 * \brief The class DummySolver is a mock solver, that is used to
 *        test \a OptimizationSolver.
 *
 * The rationale of this mock solver is:
 * "Closer the fastener from Point(0,0) is, higher the fastener's load is."
 *
 * This will encourage the tested OptimizationSolver to find
 * the best design solution having fasteners as far as possible
 * from Point(0,0).
 *
 * \remark The DummySolver is also tested ;)
 */

DummySolver::DummySolver(QObject *parent) : ISolver(parent)
{
}

DummySolver::~DummySolver()
{
}

QList<Tensor> DummySolver::calculate(const Splice *splice)
{
    Q_ASSERT(splice);
    QList<Tensor> res;

    Length maxDistance = 0.0 *m;
    const int count = splice->fastenerCount();

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);

        Length squaredDistance =
                boost::units::sqrt(
                    boost::units::pow<2>(f.positionX) +
                    boost::units::pow<2>(f.positionY));

        if (maxDistance < squaredDistance) {
            maxDistance = squaredDistance;
        }
    }

    if (maxDistance < 0.001 *m) {
        maxDistance = 0.001 *m;
    }

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);

        Length distance =
                boost::units::sqrt(
                    boost::units::pow<2>(f.positionX) +
                    boost::units::pow<2>(f.positionY));

        /* k is between 1. (closest) and 0.000001 (farthest). */
        qreal k = (maxDistance - 0.99 * distance) / maxDistance;
        Q_ASSERT((1. >= k) && (k > 0.));

        Tensor t;
        t.force_x = k * splice->appliedLoad().force_x;
        t.force_y = k * splice->appliedLoad().force_x;
        t.torque_z = k * splice->appliedLoad().torque_z;

        res.append(t);
    }
    return res;
}

