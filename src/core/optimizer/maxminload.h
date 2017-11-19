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

#ifndef CORE_OPTIMISATION_MAX_MIN_LOAD_H
#define CORE_OPTIMISATION_MAX_MIN_LOAD_H

#include <QtCore/QtContainerFwd> /* Forward Declarations of the Qt's Containers */

#include <Core/Units/UnitSystem>
class Tensor;

Force maxLoad(const QList<Tensor> result);


#endif // CORE_OPTIMISATION_MAX_MIN_LOAD_H
