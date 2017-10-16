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

#include <Core/Units/UnitSystem>
#include <Core/Units/AreaMomentOfInertia>

#include <boost/units/cmath.hpp>   /* pow() */
#include <QtCore/QDebug>

using namespace boost;
using namespace units;

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

    /* plus la distance du fastener au point (0,0) est grande,*/
    /* plus faible est la force. */
/*
    const int count = splice->fastenerCount();

    Fastener bnd; // boundary
    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);

        Length posX = boost::units::abs(f.positionX);
        Length posY = boost::units::abs(f.positionY);

        if (bnd.positionX < posX) {
            bnd.positionX = posX;
        }
        if (bnd.positionY < posY) {
            bnd.positionY = posY;
        }
    }

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);

        Length posX = boost::units::abs(f.positionX);
        Length posY = boost::units::abs(f.positionY);

        Tensor fastenerload;
        fastenerload.force_x = ((bnd.positionX - posX) / bnd.positionX ) * 1000.*N;
        fastenerload.force_y = ((bnd.positionY - posY) / bnd.positionY ) * 1000.*N;
        fastenerload.torque_z = 0.0 *si::newton_meters;

        res.append(fastenerload);
    }*/
    return res;
}

