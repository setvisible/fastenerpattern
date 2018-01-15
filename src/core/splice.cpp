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

#include "splice.h"

#include <Core/Units/UnitSystem>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif


/*! \class Splice
 *  \brief The class Splice is a container for splice document.
 *
 * Use read() and write() to serialize to JSON format.
 */

Splice::Splice()
  : m_title(QString())
  , m_author(QString())
  , m_date(QString())
  , m_description(QString())
  , m_appliedLoad(Tensor())
{
}

/******************************************************************************
 ******************************************************************************/
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
    QJsonArray spacesArray = json["designspaces"].toArray();
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
    json["designspaces"] = spacesArray;

}

/******************************************************************************
 ******************************************************************************/
QString Splice::title() const
{
    return m_title;
}

void Splice::setTitle(const QString &title)
{
    m_title = title;
}

/******************************************************************************
 ******************************************************************************/
QString Splice::author() const
{
    return m_author;
}

void Splice::setAuthor(const QString &author)
{
    m_author = author;
}

/******************************************************************************
 ******************************************************************************/
QString Splice::date() const
{
    return m_date;
}

void Splice::setDate(const QString &date)
{
    m_date = date;
}

/******************************************************************************
 ******************************************************************************/
QString Splice::description() const
{
    return m_description;
}
void Splice::setDescription(const QString &description)
{
    m_description = description;
}

/******************************************************************************
 ******************************************************************************/
Tensor Splice::appliedLoad() const
{
    return m_appliedLoad;
}

void Splice::setAppliedLoad(const Tensor &loadcase)
{
    m_appliedLoad = loadcase;
}

/******************************************************************************
 ******************************************************************************/
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
    if (index <= 0) {
        m_fasteners.insert(0, fastener);
    } else if (index >= m_fasteners.size()) {
        m_fasteners.insert(m_fasteners.size(), fastener);
    } else {
        m_fasteners.insert(index, fastener);
    }
}

void Splice::addFastener(const Fastener &fastener)
{
    m_fasteners.append(fastener);
}

void Splice::addFastener(const QVector<Fastener> &fasteners)
{
#if QT_VERSION >= 0x050500
    m_fasteners.append(fasteners);
#else
    foreach (auto f, fasteners) {
        m_fasteners.append(f);
    }
#endif
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

/******************************************************************************
 ******************************************************************************/
int Splice::designSpaceCount() const
{
    return m_designSpaces.count();
}

const DesignSpace &Splice::designSpaceAt(const int index) const
{
    return m_designSpaces.at(index);
}

void Splice::insertDesignSpace(const int index, const DesignSpace &designSpace)
{
    if (index <= 0) {
        m_designSpaces.insert(0, designSpace);
    } else if (index >= m_designSpaces.size()) {
        m_designSpaces.insert(m_designSpaces.size(), designSpace);
    } else {
        m_designSpaces.insert(index, designSpace);
    }
}

void Splice::addDesignSpace(const DesignSpace &designSpace)
{
    m_designSpaces.append(designSpace);
}

void Splice::addDesignSpace(const QVector<DesignSpace> &designSpaces)
{
#if QT_VERSION >= 0x050500
    m_designSpaces.append(designSpaces);
#else
    foreach (auto ds, designSpaces) {
        m_designSpaces.append(ds);
    }
#endif
}

void Splice::setDesignSpaceAt(const int index, const DesignSpace &designSpace)
{
    m_designSpaces[index] = designSpace; // replace() ?
}

void Splice::removeDesignSpaceAt(const int index)
{
    m_designSpaces.removeAt(index);
}

void Splice::removeAllDesignSpaces()
{
    m_designSpaces.clear();
}


/******************************************************************************
 ******************************************************************************/
bool Splice::operator==(const Splice &other) const
{
    return (*this).m_title == other.m_title
            && (*this).m_author == other.m_author
            && (*this).m_date == other.m_date
            && (*this).m_description == other.m_description
            && (*this).m_appliedLoad == other.m_appliedLoad
            && (*this).m_fasteners == other.m_fasteners
            && (*this).m_designSpaces == other.m_designSpaces;

}
bool Splice::operator!=(const Splice &other) const
{
    return ((*this) == other) ? false : true;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Equivalent means that the splices are equal,
 *   however the order of the fasteners and design spaces
 *   stored in their containers can be different.
 */
bool Splice::isEquivalentTo(const Splice &other) const
{
    /* Compare the fasteners */
    if ((*this).m_fasteners.count() != other.m_fasteners.count()) {
        return false;
    }
    {
        QVector<Fastener> list = other.m_fasteners;
        for (int i = 0; i < (*this).m_fasteners.count(); ++i) {
            const Fastener &item = (*this).m_fasteners.at(i);
            if (!list.removeOne(item)) {
                return false;
            }
        }
        Q_ASSERT(list.count() == 0);
    }

    /* Compare the design spaces */
    if ((*this).m_designSpaces.count() != other.m_designSpaces.count()) {
        return false;
    }
    {
        QVector<DesignSpace> list = other.m_designSpaces;
        for (int i = 0; i < (*this).m_designSpaces.count(); ++i) {
            const DesignSpace &item = (*this).m_designSpaces.at(i);
            if (!list.removeOne(item)) {
                return false;
            }
        }
        Q_ASSERT(list.count() == 0);
    }

    /* Compare the other properties */
    return (*this).m_title == other.m_title
            && (*this).m_author == other.m_author
            && (*this).m_date == other.m_date
            && (*this).m_description == other.m_description
            && (*this).m_appliedLoad == other.m_appliedLoad;
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const Splice &splice)
{
    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString( QString("<Splice '%0' applied=(%1N, %2N, %3Nm) fCount=%4 dCount=%5 ... >")
                     .arg(splice.title())
                     .arg(splice.appliedLoad().force_x.value() , 0, 'f', 1)
                     .arg(splice.appliedLoad().force_y.value() , 0, 'f', 1)
                     .arg(splice.appliedLoad().torque_z.value() , 0, 'f', 1)
                     .arg(splice.fastenerCount())
                     .arg(splice.designSpaceCount()) );
}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const Splice &splice)
{
    dbg.nospace() << QString("<Splice '%0' applied=(%1N, %2N, %3Nm) fCount=%4 dCount=%5 ... >")
                     .arg(splice.title())
                     .arg(splice.appliedLoad().force_x.value() , 0, 'f', 1)
                     .arg(splice.appliedLoad().force_y.value() , 0, 'f', 1)
                     .arg(splice.appliedLoad().torque_z.value() , 0, 'f', 1)
                     .arg(splice.fastenerCount())
                     .arg(splice.designSpaceCount());
    return dbg.maybeSpace();
}
#endif

