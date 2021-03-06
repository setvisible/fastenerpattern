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

#include "tensor.h"
#include <Math/Utils>

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QJsonObject>

#include <boost/units/cmath.hpp> /* pow(), sqrt() */

#ifdef QT_DEBUG
#include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#include <QtTest/QTest>
#endif


Tensor::Tensor()
    : force_x(Force())
    , force_y(Force())
    , torque_z(Torque())
{
}

Tensor::Tensor(const Force &fx,
               const Force &fy,
               const Torque &mz)
    : force_x(  fx )
    , force_y(  fy )
    , torque_z( mz )
{
}

/******************************************************************************
 ******************************************************************************/
/* JSON Serialization */
/*! \brief Assign the Tensor's members values from the given \a json object.
 */
void Tensor::read(const QJsonObject &json)
{
    force_x  = json["fx"].toDouble() *N;
    force_y  = json["fy"].toDouble() *N;
    torque_z = json["mz"].toDouble() *N_m;
}

/*! \brief Assigns the values from the Tensor to the given \a json object.
 */
void Tensor::write(QJsonObject &json) const
{
    json["fx"] = force_x.value();
    json["fy"] = force_y.value();
    json["mz"] = torque_z.value();
}

/******************************************************************************
 ******************************************************************************/
bool Tensor::operator==(const Tensor &other) const
{
    return Math::Utils::fuzzyCompare((*this).force_x.value(), other.force_x.value())
            && Math::Utils::fuzzyCompare((*this).force_y.value(), other.force_y.value())
            && Math::Utils::fuzzyCompare((*this).torque_z.value(), other.torque_z.value());

}
bool Tensor::operator!=(const Tensor &other) const
{
    return ((*this) == other) ? false : true;
}


/******************************************************************************
 ******************************************************************************/
Tensor Tensor::around(const int precision) const
{
    Tensor t;
    t.force_x = Math::Utils::round(this->force_x.value(), precision) *N;
    t.force_y = Math::Utils::round(this->force_y.value(), precision) *N;
    t.torque_z = Math::Utils::round(this->torque_z.value(), precision) *N_m;
    return t;
}

Force Tensor::resultantFxy() const
{
    return boost::units::sqrt(
                boost::units::pow<2>(this->force_x) +
                boost::units::pow<2>(this->force_y));
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const Tensor &load)
{
    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString( QString("<Tensor Fx=%0N Fy=%1N Mz=%2Nm>")
                     .arg( load.force_x.value() , 0, 'f', 3)
                     .arg( load.force_y.value() , 0, 'f', 3)
                     .arg( load.torque_z.value(), 0, 'f', 6));
}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const Tensor &load)
{
    dbg.nospace() << QString("<Tensor Fx=%0N Fy=%1N Mz=%2Nm>")
                     .arg( load.force_x.value() , 0, 'f', 3)
                     .arg( load.force_y.value() , 0, 'f', 3)
                     .arg( load.torque_z.value(), 0, 'f', 6);
    return dbg.maybeSpace();
}
#endif

