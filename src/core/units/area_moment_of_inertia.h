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

#ifndef CORE_AREA_MOMENT_OF_INERTIA_SYSTEM_H
#define CORE_AREA_MOMENT_OF_INERTIA_SYSTEM_H

#include <boost/units/static_constant.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/systems/si/length.hpp>

namespace boost {

namespace units {

/* PHYSICAL DIMENSION */
/// derived dimension for area moment of inertia
/// (also as known as second moment of area)
///  in mechanical units : L^4
typedef derived_dimension<length_base_dimension,4>::type      area_moment_of_inertia_dimension;


/* SYSTEMS */
namespace si {

typedef unit<area_moment_of_inertia_dimension,si::system>     area_moment_of_inertia;

BOOST_UNITS_STATIC_CONSTANT(quadratic_meter, area_moment_of_inertia);
BOOST_UNITS_STATIC_CONSTANT(quadratic_meters, area_moment_of_inertia);


} // namespace si

} // namespace units

} // namespace boost


#endif // CORE_AREA_MOMENT_OF_INERTIA_SYSTEM_H
