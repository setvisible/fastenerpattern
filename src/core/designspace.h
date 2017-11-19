/* - DesignSpacePattern - Copyright (C) 2016-2017 Sebastien Vavassori
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

#ifndef CORE_DESIGN_SPACE_H
#define CORE_DESIGN_SPACE_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtGui/QPolygonF>

QT_BEGIN_NAMESPACE
class QDebug;
class QJsonObject;
QT_END_NAMESPACE

class DesignSpace
{
public:
    explicit DesignSpace();

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool operator==(const DesignSpace &other) const;
    bool operator!=(const DesignSpace &other) const;

    QString name;
    QPolygonF polygon; // Hack: QPointF are expressed in meters
    /// \todo DesignSpace: replace QPointF with a quantity in meters.

};

#ifdef QT_TESTLIB_LIB
char *toString(const DesignSpace &space);
#endif

Q_DECLARE_METATYPE(DesignSpace)

#ifdef QT_DEBUG
QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const DesignSpace &space);
QT_END_NAMESPACE
#endif

#endif // CORE_DESIGN_SPACE_H
