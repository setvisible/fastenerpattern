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

#ifndef CORE_FASTENER_H
#define CORE_FASTENER_H

#include "units/unit_system.h"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QString>

class QJsonObject;

class Fastener
{
public:
    enum DOF { Fixed = 1, Free };

    explicit Fastener();
    explicit Fastener(const Length &_position_x,
                      const Length &_position_y,
                      const Length &_diameter,
                      const Length &_thickness,
                      const DOF _dof_x = Fixed,
                      const DOF _dof_y = Fixed);

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool operator==(const Fastener &other) const;
    bool operator!=(const Fastener &other) const;

    QString name;
    Length positionX;
    Length positionY;
    Length diameter;
    Length thickness;
    DOF DoF_X;
    DOF DoF_Y;

    static DOF boolToDOF(bool value);
    static bool DOFtoBool(DOF value);
};

#ifdef QT_TESTLIB_LIB
char *toString(const Fastener &fastener);
#endif

Q_DECLARE_METATYPE(Fastener)

#ifdef QT_DEBUG
QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const Fastener &fastener);
QT_END_NAMESPACE
#endif

#endif // CORE_FASTENER_H
