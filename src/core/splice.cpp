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

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>


/*! \class Splice
 *  \brief The class Splice is a container for splice document.
 *
 * Use read() and write() to serialize to JSON format.
 */

Splice::Splice(QObject *parent) : QObject(parent)
{
}

/* JSON Serialization */
/*! \brief Assign the Splice's members values from the given \a json object.
 */
void Splice::read(const QJsonObject &json)
{
    m_title = json["title"].toString();
    m_author = json["author"].toString();
    m_date = json["date"].toString();
    m_description = json["description"].toString();

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

    m_designSpaces.clear();
    QJsonArray spacesArray = json["design-spaces"].toArray();
    for (int i = 0; i < spacesArray.size(); ++i) {
        QJsonObject spaceObject = spacesArray[i].toObject();
        DesignSpace space;
        space.read(spaceObject);
        m_designSpaces.append(space);
    }
}

/*! \brief Assigns the values from the Splice to the given \a json object.
 */
void Splice::write(QJsonObject &json) const
{
    json["title"] = m_title;
    json["author"] = m_author;
    json["date"] = m_date;
    json["description"] = m_description;

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

    QJsonArray spacesArray;
    foreach (const DesignSpace space, m_designSpaces) {
        QJsonObject spaceObject;
        space.write(spaceObject);
        spacesArray.append(spaceObject);
    }
    json["design-spaces"] = spacesArray;

}

/**********************************************************************
 **********************************************************************/
QString Splice::title() const
{
    return m_title;
}

void Splice::setTitle(const QString &title)
{
    m_title = title;
}

/**********************************************************************
 **********************************************************************/
QString Splice::author() const
{
    return m_author;
}

void Splice::setAuthor(const QString &author)
{
    m_author = author;
}

/**********************************************************************
 **********************************************************************/
QString Splice::date() const
{
    return m_date;
}

void Splice::setDate(const QString &date)
{
    m_date = date;
}

/**********************************************************************
 **********************************************************************/
QString Splice::description() const
{
    return m_description;
}
void Splice::setDescription(const QString &description)
{
    m_description = description;
}

/**********************************************************************
 **********************************************************************/
Tensor Splice::appliedLoad() const
{
    return m_appliedLoad;
}

void Splice::setAppliedLoad(const Tensor &loadcase)
{
    m_appliedLoad = loadcase;
}

/**********************************************************************
 **********************************************************************/
int Splice::fastenerCount() const
{
    return m_fasteners.count();
}

const Fastener& Splice::fastenerAt(const int index) const
{
    return m_fasteners.at(index);
}

void Splice::insertFastener(const int index, const Fastener &fastener)
{
    m_fasteners.insert(index, fastener);
}

void Splice::addFastener(const Fastener &fastener)
{
    m_fasteners.append(fastener);
}

void Splice::addFastener(const QList<Fastener> &fasteners)
{
    m_fasteners.append(fasteners);
}

void Splice::setFastenerAt(const int index, const Fastener &fastener)
{
    m_fasteners[index] = fastener;
}

void Splice::removeFastenerAt(const int index)
{
    m_fasteners.removeAt(index);
}

void Splice::removeAllFasteners()
{
    m_fasteners.clear();
}

/**********************************************************************
 **********************************************************************/
QList<DesignSpace> Splice::designSpaces() const
{
    return m_designSpaces;
}

void Splice::setDesignSpaces(const QList<DesignSpace> &spaces)
{
    m_designSpaces = spaces;
}

