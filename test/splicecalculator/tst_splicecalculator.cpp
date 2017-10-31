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

#include <Core/SpliceCalculator>

#include <QtTest/QtTest>
#include <QtCore/QDebug>
#include <QtCore/QList>

#include <iostream>

using namespace boost;
using namespace units;
using namespace si;


class tst_SpliceCalculator : public QObject
{
    Q_OBJECT

private slots:
    /* General */
    void test_clear();

    /* Fasteners */
    void test_fastenerCount();
    void test_insertFastener();
    void test_insertFastener_limit();
    void test_setFastener();
    void test_removeFastener();

    /* Applied Load */
    void test_setAppliedLoad();

    /* Misc. */
    void test_setFastenerSelection();

};


/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_clear()
{

    // Given
    SpliceCalculator target;
    target.setAppliedLoad( Tensor( 100.*N, 0.*N, 0.*N_m ) );
    target.insertFastener(0, Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));
    target.insertFastener(1, Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));
    QSet<int> input = {0,1,5,9,10};
    target.setFastenerSelection( input );

    // When
    target.clear();

    // Then
    QCOMPARE( target.fastenerCount(), 0);
    QCOMPARE( target.appliedLoad(), Tensor() );
    QCOMPARE( target.selectedFastenerIndexes().empty(), true);
}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_fastenerCount()
{
    // Given, When
    SpliceCalculator target;

    // Then
      /* No results */
    QCOMPARE( target.fastenerCount(), 0);
    QCOMPARE( target.fastenerAt(-1), Fastener() );
    QCOMPARE( target.fastenerAt(0), Fastener() );
    QCOMPARE( target.fastenerAt(1), Fastener() );
    QCOMPARE( target.fastenerAt(2), Fastener() );
    QCOMPARE( target.fastenerAt(1000), Fastener() );
}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_insertFastener()
{
    // Given
    SpliceCalculator target;

    // When
    target.insertFastener(0, Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm )); /* A */
    target.insertFastener(0, Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm )); /* insert B before A */

    // Then
    QCOMPARE( target.fastenerCount(), 2);
    QCOMPARE( target.fastenerAt(-1), Fastener() );
    QCOMPARE( target.fastenerAt(0), Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) ); /* B */
    QCOMPARE( target.fastenerAt(1), Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) ); /* A */
    QCOMPARE( target.fastenerAt(2), Fastener() );
    QCOMPARE( target.fastenerAt(1000), Fastener() );
}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_insertFastener_limit()
{
    // Given
    SpliceCalculator target;

    // When
    target.insertFastener(-10, Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm )); /* prepend */
    target.insertFastener(1000, Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm )); /* append */

    // Then
    QCOMPARE( target.fastenerCount(), 2);
    QCOMPARE( target.fastenerAt(-10), Fastener() );
    QCOMPARE( target.fastenerAt(-1), Fastener() );
    QCOMPARE( target.fastenerAt(0), Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) );
    QCOMPARE( target.fastenerAt(1), Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) );
    QCOMPARE( target.fastenerAt(2), Fastener() );
    QCOMPARE( target.fastenerAt(999), Fastener() );
    QCOMPARE( target.fastenerAt(1000), Fastener() );
    QCOMPARE( target.fastenerAt(1001), Fastener() );
}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_setFastener()
{
    // Given
    SpliceCalculator target;
    target.insertFastener(0, Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));
    target.insertFastener(1, Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));

    // When
    target.setFastener(0, Fastener(  3.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));

    // Then
    QCOMPARE( target.fastenerCount(), 2);
    QCOMPARE( target.fastenerAt(0), Fastener(  3.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) );

}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_removeFastener()
{
    // Given, When
    SpliceCalculator target;
    target.insertFastener(0, Fastener(  1.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));
    target.insertFastener(1, Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ));

    // Then
    QCOMPARE( target.fastenerCount(), 2);

    target.removeFastener(-1);
    QCOMPARE( target.fastenerCount(), 2);

    target.removeFastener(100);
    QCOMPARE( target.fastenerCount(), 2);

    target.removeFastener(2);
    QCOMPARE( target.fastenerCount(), 2);

    target.removeFastener(0);
    QCOMPARE( target.fastenerCount(), 1);
    QCOMPARE( target.fastenerAt(0), Fastener(  2.*_mm, 0.*_mm, 4.83*_mm, 3.*_mm ) );
    QCOMPARE( target.fastenerAt(1), Fastener() );

    target.removeFastener(0);
    QCOMPARE( target.fastenerCount(), 0);

    target.removeFastener(0);
    QCOMPARE( target.fastenerCount(), 0);
}



/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_setAppliedLoad()
{
    // Given
    SpliceCalculator target;

    // When
    Tensor expected( 100.*N, 0.*N, 0.*N_m );
    target.setAppliedLoad( expected );
    Tensor actual = target.appliedLoad();

    // Then
    QCOMPARE( actual, expected);

}

/*************************************************************************
 *************************************************************************/
void tst_SpliceCalculator::test_setFastenerSelection()
{
    // Given
    SpliceCalculator target;

    // When
    QSet<int> expected = {0,1,5,9,10};
    target.setFastenerSelection( expected );
    QSet<int> actual = target.selectedFastenerIndexes();

    // Then
    QCOMPARE( actual, expected);
}


QTEST_APPLESS_MAIN(tst_SpliceCalculator)

#include "tst_splicecalculator.moc"

