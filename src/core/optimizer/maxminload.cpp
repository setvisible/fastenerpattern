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

#include "maxminload.h"

#include <Core/Tensor>

#include <QtCore/QList>

Force maxLoad(const QList<Tensor> result)
{
    Force maxResultantForce = 0.0 *N;
    for (int j = 0; j < result.count(); ++j) {
        const Tensor f = result.at(j);
        const Force resultant = f.resultantFxy();
        if (maxResultantForce < resultant) {
            maxResultantForce = resultant;
        }
    }
    return maxResultantForce;
}

