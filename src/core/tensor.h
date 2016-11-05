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

#ifndef CORE_TENSOR_H
#define CORE_TENSOR_H

#include "units/unit_system.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaType>

class QJsonObject;

class Tensor
{
public:
    explicit Tensor();
    explicit Tensor(const Force &fx, const Force &fy, const Torque &mz);

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool operator==(const Tensor &other) const;
    bool operator!=(const Tensor &other) const;

    Force force_x;
    Force force_y;
    Torque torque_z;
};

#ifdef QT_TESTLIB_LIB
char *toString(const Tensor &load);
#endif

Q_DECLARE_METATYPE(Tensor)

#ifdef QT_DEBUG
QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const Tensor &load);
QT_END_NAMESPACE
#endif


#endif // CORE_TENSOR_H
