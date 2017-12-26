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

#include "fastener.h"
#include <Math/Utils>

#include <QtCore/QJsonObject>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif

Fastener::Fastener()
    : name(QStringLiteral("New fastener"))
    , positionX(Length())
    , positionY(Length())
    , diameter(Length())
    , thickness(Length())
    , DoF_X(Fixed)
    , DoF_Y(Fixed)
{
}

Fastener::Fastener(const Length& _position_x,
                   const Length& _position_y,
                   const Length& _diameter,
                   const Length& _thickness,
                   const DOF _dof_x,
                   const DOF _dof_y)
    : name(QStringLiteral("New fastener"))
    , positionX(_position_x)
    , positionY(_position_y)
    , diameter(_diameter)
    , thickness(_thickness)
    , DoF_X(_dof_x)
    , DoF_Y(_dof_y)
{
}

/******************************************************************************
 ******************************************************************************/
/* JSON Serialization */
/*! \brief Assign the Fastener's members values from the given \a json object.
 */
void Fastener::read(const QJsonObject &json)
{
    name = json["name"].toString();
    positionX = json["position_x"].toDouble() *m ;
    positionY = json["position_y"].toDouble() *m ;
    diameter  = json["diameter"  ].toDouble() *m ;
    thickness = json["thickness" ].toDouble() *m ;
    DoF_X = boolToDOF( json["DoF_X"].toBool() );
    DoF_Y = boolToDOF( json["DoF_Y"].toBool() );
}

/*! \brief Assigns the values from the Fastener to the given \a json object.
 */
void Fastener::write(QJsonObject &json) const
{
    json["name"] = name;
    json["position_x"] = positionX.value() ;
    json["position_y"] = positionY.value() ;
    json["diameter"  ] = diameter.value()  ;
    json["thickness" ] = thickness.value() ;
    json["DoF_X"] = DOFtoBool( DoF_X );
    json["DoF_Y"] = DOFtoBool( DoF_Y );
}

/******************************************************************************
 ******************************************************************************/
bool Fastener::operator==(const Fastener &other) const
{
    return (*this).name == other.name
            && Math::Utils::fuzzyCompare((*this).positionX.value(), other.positionX.value())
            && Math::Utils::fuzzyCompare((*this).positionY.value(), other.positionY.value())
            && Math::Utils::fuzzyCompare((*this).diameter.value(), other.diameter.value())
            && Math::Utils::fuzzyCompare((*this).thickness.value(), other.thickness.value())
            && (*this).DoF_X == other.DoF_X
            && (*this).DoF_Y == other.DoF_Y;

}
bool Fastener::operator!=(const Fastener &other) const
{
    return ((*this) == other) ? false : true;
}

Fastener::DOF Fastener::boolToDOF(bool value)
{
    return value ? Fastener::Fixed : Fastener::Free;
}
bool Fastener::DOFtoBool(Fastener::DOF value)
{
    return (value == Fastener::Fixed) ? true : false;
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const Fastener &fastener)
{
    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString( QString("<Fastener '%0' at Point(%1,%2)(mm,mm) t=%3mm d=%4mm X=%5 Y=%6>")
                     .arg( fastener.name )
                     .arg( fastener.positionX.value()*1000. , 0, 'f', 1)
                     .arg( fastener.positionY.value()*1000. , 0, 'f', 1)
                     .arg( fastener.thickness.value()*1000. , 0, 'f', 3)
                     .arg( fastener.diameter.value()*1000. , 0, 'f', 3)
                     .arg( Fastener::DOFtoBool(fastener.DoF_X) ?
                               QLatin1String("fixed") : QLatin1String("free"))
                     .arg( Fastener::DOFtoBool(fastener.DoF_Y) ?
                               QLatin1String("fixed") : QLatin1String("free")) );
}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const Fastener &fastener)
{
    dbg.nospace() << QString("<Fastener '%0' at Point(%1,%2)(mm,mm) t=%3mm d=%4mm X=%5 Y=%6>")
                     .arg( fastener.name )
                     .arg( fastener.positionX.value()*1000. , 0, 'f', 1)
                     .arg( fastener.positionY.value()*1000. , 0, 'f', 1)
                     .arg( fastener.thickness.value()*1000. , 0, 'f', 3)
                     .arg( fastener.diameter.value()*1000. , 0, 'f', 3)
                     .arg( Fastener::DOFtoBool(fastener.DoF_X) ?
                               QLatin1String("fixed") : QLatin1String("free"))
                     .arg( Fastener::DOFtoBool(fastener.DoF_Y) ?
                               QLatin1String("fixed") : QLatin1String("free"));
    return dbg.maybeSpace();
}
#endif
