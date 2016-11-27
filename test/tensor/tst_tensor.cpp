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

#include <Core/Tensor>

#include <QtTest/QtTest>

class tst_Tensor : public QObject
{
    Q_OBJECT
private slots:
    void test_around();

};

/*************************************************************************
 *************************************************************************/
void tst_Tensor::test_around()
{
    // Given, When
    Tensor t( 1./3. *N, 1./3. *N, 2./3. *N_m);

    // Then
    QCOMPARE( t, Tensor(1./3. *N, 1./3. *N, 2./3. *N_m) );
    QCOMPARE( t.around(), Tensor(0.333 *N, 0.333 *N, 0.667 *N_m) );

    QCOMPARE( t.around( 1000), t.around(9) );
    QCOMPARE( t.around(    9), Tensor(0.333333333 *N, 0.333333333 *N, 0.666666667 *N_m) );
    QCOMPARE( t.around(    5), Tensor(0.33333 *N, 0.33333 *N, 0.66667 *N_m) );
    QCOMPARE( t.around(    2), Tensor(0.33 *N, 0.33 *N, 0.67 *N_m) );
    QCOMPARE( t.around(    0), Tensor(0. *N, 0. *N, 1. *N_m) );
    QCOMPARE( t.around(   -1), Tensor(0. *N, 0. *N, 1. *N_m) );
    QCOMPARE( t.around(-1000), Tensor(0. *N, 0. *N, 1. *N_m) );
}


QTEST_APPLESS_MAIN(tst_Tensor)

#include "tst_tensor.moc"

