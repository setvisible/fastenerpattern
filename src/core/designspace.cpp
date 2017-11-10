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

#include "designspace.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif

DesignSpace::DesignSpace()
    : name(QStringLiteral("New Design Space"))
{
}

/* JSON Serialization */
void DesignSpace::read(const QJsonObject &json)
{
    name = json["name"].toString();
    polygon.clear();
    QJsonArray pointsArray = json["points"].toArray();
    for (int i = 0; i < pointsArray.size(); ++i) {
        const QJsonArray coords = pointsArray[i].toArray();
        polygon << QPointF(coords[0].toDouble(), coords[1].toDouble());
    }
}

void DesignSpace::write(QJsonObject &json) const
{
    json["name"] = name;
    QJsonArray pointsArray;
    foreach (const QPointF point, polygon) {
        QJsonArray coords;
        coords << point.x() << point.y();
        pointsArray.append(coords);
    }
    json["points"] = pointsArray;
}

bool DesignSpace::operator==(const DesignSpace &other) const
{

    return (*this).name == other.name
            && (*this).polygon == other.polygon;
}

bool DesignSpace::operator!=(const DesignSpace &other) const
{
    return ((*this) == other) ? false : true;
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const DesignSpace &space)
{
    QString str;
    str += QString("<DesignSpace '%0' at").arg( space.name );
    foreach (const QPointF &point, space.polygon) {
        str += QString(" (%0, %1)")
                .arg(point.x()*1000. , 0, 'f', 1)
                .arg(point.y()*1000. , 0, 'f', 1);
    }
    str += QLatin1String(" (mm,mm)>");

    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString( str );

}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const DesignSpace &space)
{
    QString str;
    str += QString("<DesignSpace '%0' at").arg( space.name );
    foreach (const QPointF &point, space.polygon) {
        str += QString(" (%0, %1)")
                .arg(point.x()*1000. , 0, 'f', 1)
                .arg(point.y()*1000. , 0, 'f', 1);
    }
    str += QLatin1String(" (mm,mm)>");

    dbg.nospace() << str;
    return dbg.maybeSpace();
}
#endif

