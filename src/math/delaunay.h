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

#ifndef MATH_DELAUNAY_H
#define MATH_DELAUNAY_H

#include <QtCore/QtContainerFwd> /* Forward Declarations of the Qt's Containers */

QT_BEGIN_NAMESPACE
class QLineF;
class QPointF;
QT_END_NAMESPACE

namespace Math {

QList<QLineF> delaunayTriangulation(const QList<QPointF> &points);

}

#endif // MATH_DELAUNAY_H
