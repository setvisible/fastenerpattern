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

#ifndef CORE_UNIT_SYSTEM_H
#define CORE_UNIT_SYSTEM_H

#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/systems/si/area.hpp>
#include <boost/units/systems/si/force.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/pressure.hpp>
#include <boost/units/systems/si/torque.hpp>
#include <boost/units/systems/si/volume.hpp>

#include <boost/units/systems/si/prefixes.hpp>  // kilo, milli,...


/* We define here some additional aliases for mechanical units */
BOOST_UNITS_STATIC_CONSTANT( N,   boost::units::si::force);
BOOST_UNITS_STATIC_CONSTANT( m,   boost::units::si::length);
BOOST_UNITS_STATIC_CONSTANT( Pa,  boost::units::si::pressure);
BOOST_UNITS_STATIC_CONSTANT( N_m, boost::units::si::torque);

//typedef boost::units::si::square_meters m_2;
#define m_2     boost::units::si::square_meters
#define mm_2    0.000001*m_2

#define m_3     boost::units::si::cubic_meters

//typedef boost::units::si::newton N;
//#define N       boost::units::si::newtons
#define daN     10.*N
#define kN      1000.*N

//typedef boost::units::si::meter_base_unit m;
//#define m       boost::units::si::meters
/* Namespace to avoid names collision */
/* Ex : "mm" is millimeters and also the memory-manager of GeCode */
// mm == memory-manager or millimeter --> collision !
#define _mm      0.001*m
// ? namespace fp_units { mm; }

//#define N_m     boost::units::si::newton_meters
#define N_mm    0.001*N_m

//#define Pa      boost::units::si::pascals
#define MPa     1000000.*Pa
#define GPa     1000000000.*Pa


/* Custom Types */
typedef boost::units::quantity<boost::units::si::force> Force;
typedef boost::units::quantity<boost::units::si::length> Length;
typedef boost::units::quantity<boost::units::si::torque> Torque;




#endif // CORE_UNIT_SYSTEM_H
