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
#include "global.h"

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QtMath>   /* qPow() */

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

bool Tensor::operator==(const Tensor &other) const
{
    return fuzzyCompare((*this).force_x.value(), other.force_x.value())
            && fuzzyCompare((*this).force_y.value(), other.force_y.value())
            && fuzzyCompare((*this).torque_z.value(), other.torque_z.value());

}
bool Tensor::operator!=(const Tensor &other) const
{
    return ((*this) == other) ? false : true;
}


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

static qreal round(qreal f, int precision)
{
    if (precision < 0) precision = 0;
    if (precision > 9) precision = 9;
    qreal significantDigits = qPow(10, precision);
    qreal round = qFloor((f * significantDigits) + 0.5) / significantDigits;
    return round;
}

Tensor Tensor::around(const int precision) const
{
    Tensor t;
    t.force_x = round(this->force_x.value(), precision) *N;
    t.force_y = round(this->force_y.value(), precision) *N;
    t.torque_z = round(this->torque_z.value(), precision) *N_m;
    return t;
}

#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const Tensor &load)
{
    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString( QString("Load<%0 %1 %2>")
                     .arg( load.force_x.value() , 0, 'f', 3)
                     .arg( load.force_y.value() , 0, 'f', 3)
                     .arg( load.torque_z.value(), 0, 'f', 3));
}
#endif


#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const Tensor &load)
{
    dbg.nospace() << QString("Load<%0 %1 %2>")
                     .arg( load.force_x.value() , 0, 'f', 3)
                     .arg( load.force_y.value() , 0, 'f', 3)
                     .arg( load.torque_z.value(), 0, 'f', 3);
    return dbg.maybeSpace();
}
#endif


