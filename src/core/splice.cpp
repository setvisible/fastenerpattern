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

#include "splice.h"

#include <Core/Units/UnitSystem>

#include <QJsonArray>
#include <QJsonObject>


/*! \class Splice
 *  \brief The class Splice is a container for splice document.
 *
 * Use read() and write() to serialize to JSON format.
 */

Splice::Splice(QObject *parent) : QObject(parent)
{
}

/* JSON Serialization */
void Splice::read(const QJsonObject &json)
{
    m_name = json["name"].toString();
    m_author = json["author"].toString();

    QJsonObject load = json["load"].toObject();
    m_appliedLoad.read(load);

    m_fasteners.clear();
    QJsonArray fixationsArray = json["fasteners"].toArray();
    for (int i = 0; i < fixationsArray.size(); ++i) {
        QJsonObject fixationObject = fixationsArray[i].toObject();
        Fastener fixation;
        fixation.read(fixationObject);
        m_fasteners.append(fixation);
    }

    /// \todo  m_frontier.clear();
    /// \todo  QJsonArray frontierArray = json["frontier"].toArray();
    /// \todo  for (int i = 0; i < frontierArray.size(); ++i) {
    /// \todo      QJsonObject frontierObject = frontierArray[i].toVariant();
    /// \todo      frontierObject.toVariant
    /// \todo      Fixation vec;
    /// \todo      vec.read(frontierObject);
    /// \todo      m_frontier.append(vec);
    /// \todo  }
}

void Splice::write(QJsonObject &json) const
{
    json["name"] = m_name;
    json["author"] = m_author;

    QJsonObject load;
    m_appliedLoad.write(load);
    json["load"] = load;

    QJsonArray fixationsArray;
    foreach (const Fastener fixation, m_fasteners) {
        QJsonObject fixationObject;
        fixation.write(fixationObject);
        fixationsArray.append(fixationObject);
    }
    json["fasteners"] = fixationsArray;

}

QString Splice::name() const
{
    return m_name;
}
void Splice::setName(const QString &name)
{
    m_name = name;
}

QString Splice::author() const
{
    return m_author;
}
void Splice::setAuthor(const QString &author)
{
    m_author = author;
}

Tensor Splice::appliedLoad() const
{
    return m_appliedLoad;
}

void Splice::setAppliedLoad(const Tensor &loadcase)
{
    m_appliedLoad = loadcase;
}

QList<Fastener> Splice::fasteners() const
{
    return m_fasteners;
}

void Splice::addFastener(const Fastener &fastener)
{
    m_fasteners.append(fastener);
}

void Splice::addFastener(const QList<Fastener> &fasteners)
{
    m_fasteners.append(fasteners);
}

void Splice::removeAllFasteners()
{
    m_fasteners.clear();
}

