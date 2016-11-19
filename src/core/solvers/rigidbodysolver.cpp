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

#include "rigidbodysolver.h"

#include <Core/Units/UnitSystem>
#include <Core/Units/AreaMomentOfInertia>

#include <boost/units/cmath.hpp>   /* pow() */
#include <QtCore/QDebug>

using namespace boost;
using namespace units;

RigidBodySolver::RigidBodySolver(QObject *parent) : ISolver(parent)
  , m_params(SolverParameters::RigidBodySolverWithIsoBearing)
{
}

RigidBodySolver::~RigidBodySolver()
{
}

SolverParameters RigidBodySolver::parameters() const
{
    return m_params;
}

void RigidBodySolver::setParameters(SolverParameters mode)
{
    if ( mode == SolverParameters::RigidBodySolverWithIsoBearing  ||
         mode == SolverParameters::RigidBodySolverWithIsoShear) {
        m_params = mode;
    } else {
        m_params = SolverParameters::NoSolver;
    }
}

QList<Tensor> RigidBodySolver::calculate(const Splice *splice)
{
    Q_ASSERT(splice);
    Q_ASSERT(m_params == SolverParameters::RigidBodySolverWithIsoBearing ||
             m_params == SolverParameters::RigidBodySolverWithIsoShear);

    QList<Tensor> res;

    const int count = splice->fastenerCount();

    // ---------------------------------
    struct Data {
        quantity<si::area> Ax;
        quantity<si::area> Ay;
        quantity<si::volume> Bx;
        quantity<si::volume> By;
    };
    struct Inertia {
        quantity<si::area_moment_of_inertia> x;
        quantity<si::area_moment_of_inertia> y;
    };
    // ---------------------------------

    QList<Data> _list;

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);
    //foreach(const Fastener &f, splice->fasteners()) {

        Data d { 0.*m_2, 0.*m_2, 0.*m_3, 0.*m_3 };

        switch (m_params) {
        case SolverParameters::RigidBodySolverWithIsoBearing:

            if (f.DoF_X == Fastener::Fixed) d.Ax = f.diameter * f.thickness;
            if (f.DoF_Y == Fastener::Fixed) d.Ay = f.diameter * f.thickness;

            break;
        case SolverParameters::RigidBodySolverWithIsoShear:

            if (f.DoF_X == Fastener::Fixed) d.Ax = f.diameter * f.diameter;
            if (f.DoF_Y == Fastener::Fixed) d.Ay = f.diameter * f.diameter;

            break;
        default:
            Q_UNREACHABLE();
            break;
        }

        d.Bx = d.Ax * f.positionY;
        d.By = d.Ay * f.positionX;

        _list.append( d );
    }

    // ---------------------------------
    Data sumData { 0, 0, 0, 0 };
    foreach(const Data &p, _list) {
        sumData.Ax += p.Ax;
        sumData.Ay += p.Ay;
        sumData.Bx += p.Bx;
        sumData.By += p.By;
    }

    quantity<si::length> CoG_x = sumData.By / sumData.Ay ;
    quantity<si::length> CoG_y = sumData.Bx / sumData.Ax ;

    // ---------------------------------
    QList<Inertia> inertias;

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);
        Inertia inertia;
        inertia.x = _list[i].Ax * boost::units::pow<2>(f.positionY - CoG_y);
        inertia.y = _list[i].Ay * boost::units::pow<2>(f.positionX - CoG_x);
        inertias.append(inertia);
    }


    Inertia sumInertia { 0, 0 };
    foreach(const Inertia &p, inertias) {
        sumInertia.x += p.x;
        sumInertia.y += p.y;
    }

    inertias.clear(); // no need anymore

    // ---------------------------------
    Tensor appliedLoad = splice->appliedLoad();
    Tensor cogLoad;
    cogLoad.force_x = appliedLoad.force_x;
    cogLoad.force_y = appliedLoad.force_y;
    cogLoad.torque_z = appliedLoad.torque_z
            + (CoG_y * appliedLoad.force_x - CoG_x * appliedLoad.force_y) / (si::radian);

    // ---------------------------------

    for (int i = 0 ; i < count ; ++i) {
        const Fastener f = splice->fastenerAt(i);

        Tensor fastenerload;
        fastenerload.force_x =
                -1.0/(sumInertia.y + sumInertia.x) * cogLoad.torque_z
                * (f.positionY - CoG_y) * _list[i].Ax * si::radians
                + cogLoad.force_x * (_list[i].Ax / sumData.Ax);

        fastenerload.force_y =
                1.0/(sumInertia.y + sumInertia.x)  * cogLoad.torque_z
                * (f.positionX - CoG_x) * _list[i].Ay * si::radians
                + cogLoad.force_y * (_list[i].Ay / sumData.Ay);

        fastenerload.torque_z = 0.0 *si::newton_meters;

        res.append(fastenerload);
    }
    return res;
}

