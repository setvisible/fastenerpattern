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

#ifndef CORE_GLOBAL_H
#define CORE_GLOBAL_H

#include <QtCore/QtGlobal>

Q_DECL_CONSTEXPR static inline bool fuzzyCompare(double p1, double p2)
{
    return qFuzzyCompare(p1, p2);
}

Q_DECL_CONSTEXPR static inline bool fuzzyCompare(float p1, float p2)
{
    return qFuzzyCompare(p1, p2);
}

#endif // CORE_GLOBAL_H
