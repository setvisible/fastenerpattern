/* - DesignSpacePattern - Copyright (C) 2016 Sebastien Vavassori
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
#include "global.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

DesignSpace::DesignSpace()
{
}

/* JSON Serialization */
void DesignSpace::read(const QJsonObject &json)
{
    polygon.clear();
    QJsonArray pointsArray = json["points"].toArray();
    for (int i = 0; i < pointsArray.size(); ++i) {
        const QJsonArray coords = pointsArray[i].toArray();
        polygon << QPointF(coords[0].toDouble(), coords[1].toDouble());
    }
}

void DesignSpace::write(QJsonObject &json) const
{
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

    return (*this).polygon == other.polygon;
}

bool DesignSpace::operator!=(const DesignSpace &other) const
{
    return ((*this) == other) ? false : true;
}

