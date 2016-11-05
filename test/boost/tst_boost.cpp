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

#include <QtTest/QtTest>
#include <QtCore/QDebug>

#include <iostream>

#include <boost/units/base_unit.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>           // name_format, symbol_format
#include <boost/units/systems/cgs.hpp>
#include <boost/units/systems/si.hpp>

namespace bu = boost::units;
using namespace bu;
using namespace si;

class tst_Boost : public QObject
{
    Q_OBJECT

private slots:
    void test_simple();
    void test_boost_output();
};


/*! Quick tests to check that Boost::Units works with Qt and QtTest.
 */
void tst_Boost::test_simple()
{
    /* Default Units */
    {
        quantity<mass> mass = 2.0 * kilogram;    /* singular form */
        QCOMPARE( mass, 2.0 * kilogrammes );     /* plurial form */
    }
    {
        quantity<si::velocity> speed = 12.3 * si::meters/si::second ;
        QCOMPARE( speed, 12.3*si::meters_per_second );
    }
    {
        quantity<velocity> speed = 12.3 * meters_per_second ;
        QCOMPARE( speed, 12.3 * meters_per_second );
    }
    {
        quantity<si::length> length = 6.15 * si::meters ;
        quantity<si::time>   time   = 0.5 * si::seconds;
        quantity<si::velocity> speed;
        speed = length / time ;
        QCOMPARE( speed, 12.3 * si::meters_per_second );
    }
    {
        quantity<force> P = 10.0 * newtons;
        quantity<area>  a =  2.0 * square_meters;
        quantity<pressure> sigma;
        sigma = P / a ;
        QCOMPARE( sigma, 5.0 * pascals );
    }
    {
        quantity<plane_angle>   theta = 0.375*radians;
        quantity<dimensionless> sin_theta = bu::sin(theta);
        quantity<plane_angle>   thetap = bu::asin(sin_theta);
        QCOMPARE( theta,   0.375 * radians);
        QCOMPARE( thetap,  0.375 * radians);
    }
    {
        quantity<electric_potential> v;
        quantity<current>            i = 0.03*amperes;
        quantity<resistance>         z = 400.0*ohms;
        v = i * z;
        QCOMPARE( v, 12.*volts);
    }
    {
        quantity<torque> tz = 10.5*newton_meter;  /* singular form */
        QCOMPARE( tz, 10.5*newton_meters );       /* plurial form */

        quantity<length> r = 2.0 * meter;
        quantity<force>  f = 3.0 * newton;
        quantity<torque> Mz = r * f * joule / radian / meter / newton;
        QCOMPARE( Mz, 6.0 *newton_meters );
    }
    {
        quantity<length> L = 2.0*meters;
        quantity<energy> E = kilograms*bu::pow<2>(L/seconds);
        QCOMPARE( E, 4.0 *joules );
    }
    {
        quantity<si::force>  F = 1.33 * si::newton;
        quantity<si::length> d = 4.55 * si::meter;
        quantity<si::torque> m_z = F * d *(1 / radian);
        QCOMPARE( m_z, 6.0515 * si::newton_meter );
    }

    /* Custom Units */
    /// \todo quantity<si::pressure> sigma_xx = 100.01*pascals;
    /// \todo quantity<si::stress> sigma_yy = 1000.0*s_pascals;
    /// \todo derived dimension for linear momentum : L M T^-1
}

/*************************************************************************
 *************************************************************************/
void tst_Boost::test_boost_output()
{
    std::cout << typename_format
              << "I*w   = " << moment_of_inertia()*angular_velocity() << std::endl
              << "I*w/L = " << moment_of_inertia()*angular_velocity()/angular_momentum() << std::endl
              << "I*w^2 = " << moment_of_inertia()*bu::pow<2>(angular_velocity()) << std::endl
              << std::endl;
    std::cout << raw_format
              << "I*w   = " << moment_of_inertia()*angular_velocity() << std::endl
              << "I*w/L = " << moment_of_inertia()*angular_velocity()/angular_momentum() << std::endl
              << "I*w^2 = " << moment_of_inertia()*bu::pow<2>(angular_velocity()) << std::endl
              << std::endl;
    std::cout << symbol_format
              << "I*w   = " << moment_of_inertia()*angular_velocity() << std::endl
              << "I*w/L = " << moment_of_inertia()*angular_velocity()/angular_momentum() << std::endl
              << "I*w^2 = " << moment_of_inertia()*bu::pow<2>(angular_velocity()) << std::endl
              << std::endl;
    std::cout << name_format
              << "I*w   = " << moment_of_inertia()*angular_velocity() << std::endl
              << "I*w/L = " << moment_of_inertia()*angular_velocity()/angular_momentum() << std::endl
              << "I*w^2 = " << moment_of_inertia()*bu::pow<2>(angular_velocity()) << std::endl
              << std::endl;

    /* Arbitrary conversions also work: */
    quantity<si::area> A(1.5*si::meter*cgs::centimeter);
    std::cout << 1.5*si::meter*cgs::centimeter << std::endl
              << A << std::endl
              << std::endl;

}


QTEST_APPLESS_MAIN(tst_Boost)

#include "tst_boost.moc"

