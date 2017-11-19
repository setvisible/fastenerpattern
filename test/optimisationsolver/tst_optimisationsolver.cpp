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

#include <Core/Optimizer/OptimisationSolver>
#include "dummysolver.h"

#include <Core/Splice>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

class tst_OptimisationSolver : public QObject
{
    Q_OBJECT

private slots:
    /* Test the test comparator */
    void test_splice_compare();

    /* Test the DummySolver */
    void test_dummy_solver_1();
    void test_dummy_solver_2();

    /* Test the OptimisationSolver */
    void test_null();
    void test_null_input();
    void test_null_solver();
    void test_null_applied_load();
    void test_null_design_space();
    void test_null_fasteners();

    void test_point();
    void test_line();
    void test_triangle();
    void test_square();

};

/******************************************************************************
 ******************************************************************************/
inline bool spliceCompare(const Splice &t1, const Splice &t2, const char *actual,
                          const char *expected, const char *file, int line)
{
    return QTest::compare_helper(
                t1.isEquivalentTo(t2), "Compared splices are not the same",
                toString(t1), toString(t2), actual, expected, file, line);
}

#define SPLICE_COMPARE(actual, expected) \
    do {\
    if (!spliceCompare(actual, expected, #actual, #expected, __FILE__, __LINE__))\
    return;\
    } while (0)


/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_splice_compare()
{
    /*************************************************************************\
    * Two Splice objects with equivalent fasteners but in different order.    *
    \*************************************************************************/

    // Given, When
    Splice splice1;
    splice1.addFastener( Fastener( 1.*_mm, 1.*_mm, 1.*_mm, 1.*_mm ) );
    splice1.addFastener( Fastener( 2.*_mm, 2.*_mm, 2.*_mm, 2.*_mm ) );
    splice1.addFastener( Fastener( 3.*_mm, 3.*_mm, 3.*_mm, 3.*_mm ) );

    Splice splice2;
    splice2.addFastener( Fastener( 3.*_mm, 3.*_mm, 3.*_mm, 3.*_mm ) );
    splice2.addFastener( Fastener( 2.*_mm, 2.*_mm, 2.*_mm, 2.*_mm ) );
    splice2.addFastener( Fastener( 1.*_mm, 1.*_mm, 1.*_mm, 1.*_mm ) );

    // Then
    /* QEXPECT_FAIL expects the next QCOMPARE or QVERIFY fails. */
    QEXPECT_FAIL("", "As expected, the splices are not the same.", Continue);
    QCOMPARE(splice1, splice2);

    /* QCOMPARE must fail but SPLICE_COMPARE must pass */
    SPLICE_COMPARE(splice1, splice2);
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_dummy_solver_1()
{
    // Given
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    splice.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    // When
    DummySolver target;
    QList<Tensor> actual = target.calculate(&splice);

    // Then
    QCOMPARE( actual.count(), 1);
    QCOMPARE( actual.at(0).around(2), Tensor( 1000.00*N, 1000.00*N, 1.000*N_m));
}

void tst_OptimisationSolver::test_dummy_solver_2()
{
    // Given
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    splice.addFastener( Fastener( 10.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    splice.addFastener( Fastener( 15.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    splice.addFastener( Fastener( 20.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    // When
    DummySolver target;
    QList<Tensor> actual = target.calculate(&splice);

    // Then
    QCOMPARE( actual.count(), 3);
    QCOMPARE( actual.at(0).around(2), Tensor( 990.10*N, 990.10*N, 0.990*N_m));
    QCOMPARE( actual.at(1).around(2), Tensor( 985.22*N, 985.22*N, 0.990*N_m));
    QCOMPARE( actual.at(2).around(2), Tensor( 980.39*N, 980.39*N, 0.980*N_m));
}


/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null()
{
    // Given
    OptimisationSolver target;
    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    // When
    target.runSync();

    // Then
    QCOMPARE(spy.count(), 3);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_UNDEFINED_SOLVER);
    QVERIFY(spy.at(1).at(0) == (int)OptimisationErrorType::ERR_UNDEFINED_INPUT_SPLICE);
    QVERIFY(spy.at(2).at(0) == (int)OptimisationErrorType::ERR_UNDEFINED_OUTPUT_SPLICE);
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null_input()
{
    // Given
    DummySolver dummy;

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput( Q_NULLPTR ); /* No input */
    target.setOutput(&actual);

    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    target.runSync();

    // Then
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_UNDEFINED_INPUT_SPLICE);

    SPLICE_COMPARE(actual, Splice());
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null_solver()
{
    // Given
    DesignSpace ds;
    ds.polygon << QPointF(0.00, 0.02)
               << QPointF(0.01, 0.03)
               << QPointF(0.02,-0.02);

    Splice input;
    input.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( Q_NULLPTR ); /* No Solver */
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    target.runSync();

    // Then
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_UNDEFINED_SOLVER);

    SPLICE_COMPARE(actual, Splice());
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null_applied_load()
{
    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF(0.00, 0.02)
               << QPointF(0.01, 0.03)
               << QPointF(0.02,-0.02);

    Splice input;
    input.setAppliedLoad( Tensor( 0.*N, 0.*N, 0.*N_mm) ); /* No Applied Load */
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    target.runSync();

    // Then
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_NO_APPLIED_LOAD);

    SPLICE_COMPARE(actual, Splice());
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null_design_space()
{
    // Given
    DummySolver dummy;

    Splice input;
    input.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    /* No Design Space */

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    target.runSync();

    // Then
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_NO_DESIGNSPACE);

    SPLICE_COMPARE(actual, Splice());

}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_null_fasteners()
{
    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF(0.00, 0.02)
               << QPointF(0.01, 0.03)
               << QPointF(0.02,-0.02);

    Splice input;
    input.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    input.addDesignSpace( ds );

    /* No Fasteners */

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    QSignalSpy spy( &target, SIGNAL(errorDetected(OptimisationErrorType)));

    target.runSync();

    // Then
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0) == (int)OptimisationErrorType::ERR_NO_FASTENER);

    SPLICE_COMPARE(actual, Splice());

}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_point()
{
    /**********************************************************************\
    * We test a model containing only one fastener                         *
    * and one point as design space.                                       *
    * The obvious best solution is the fasteners at coordinates (10., 20.) *
    \**********************************************************************/

    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF(0.01, 0.02)
               << QPointF(0.01, 0.02); /* trick */

    Splice input;
    input.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice expected;
    expected.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    expected.addDesignSpace( ds );
    expected.addFastener( Fastener( 10.*_mm, 20.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    target.runSync();

    // Then
    SPLICE_COMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_line()
{
    /**********************************************************************\
    * We test a simple model containing 2 fasteners                        *
    * and a thin line as design space.                                     *
    * The obvious best solution is fasteners at endpoints.                 *
    \**********************************************************************/

    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF( 0.01, 0.01) << QPointF( 0.01,-0.01);

    Splice input;
    input.setAppliedLoad( Tensor( 1000.*N, 0.*N, 0.*N_mm) );
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice expected;
    expected.setAppliedLoad( Tensor( 1000.*N, 0.*N, 0.*N_mm) );
    expected.addDesignSpace( ds );
    expected.addFastener( Fastener( 10.*_mm,  10.*_mm, 4.83*_mm, 2.*_mm ) );
    expected.addFastener( Fastener( 10.*_mm, -10.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    target.runSync();

    // Then
    SPLICE_COMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_triangle()
{
    /**********************************************************************\
    * We test a simple model containing only one fastener                  *
    * and setting a simple triangle as design space.                       *
    * The obvious best solution is the fastener in coordinates (10., 0.)   *
    \**********************************************************************/

    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF(0,0.001) << QPointF(0,-0.001) << QPointF(0.01,0);

    Splice input;
    input.setTitle(QLatin1String("my title"));
    input.setAuthor(QLatin1String("my author"));
    input.setDate(QLatin1String("my date"));
    input.setDescription(QLatin1String("my description"));
    input.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );


    Splice expected;
    expected.setTitle(QLatin1String("my title"));
    expected.setAuthor(QLatin1String("my author"));
    expected.setDate(QLatin1String("my date"));
    expected.setDescription(QLatin1String("my description"));
    expected.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    expected.addDesignSpace( ds );
    expected.addFastener( Fastener( 10.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    target.runSync();

    // Then
    QCOMPARE(actual.title()             , expected.title());
    QCOMPARE(actual.author()            , expected.author());
    QCOMPARE(actual.date()              , expected.date());
    QCOMPARE(actual.description()       , expected.description());
    QCOMPARE(actual.appliedLoad()       , expected.appliedLoad());
    QCOMPARE(actual.designSpaceCount()  , 1);
    QCOMPARE(actual.designSpaceAt(0)    , expected.designSpaceAt(0));
    QCOMPARE(actual.fastenerCount()     , 1);
    QCOMPARE(actual.fastenerAt(0)       , expected.fastenerAt(0));

    SPLICE_COMPARE(actual, expected);
}


/******************************************************************************
 ******************************************************************************/
void tst_OptimisationSolver::test_square()
{
    /**********************************************************************\
    * We test a simple model containing 4 fasteners                        *
    * and a 4-corners polyhedron as design space.                          *
    * The obvious best solution is fasteners at corners.                   *
    \**********************************************************************/

    // Given
    DummySolver dummy;
    DesignSpace ds;
    ds.polygon << QPointF( 0.00, 0.01)
               << QPointF( 0.01, 0.00)
               << QPointF(-0.01, 0.00)
               << QPointF( 0.00,-0.01);

    Splice input;
    input.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    input.addDesignSpace( ds );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );
    input.addFastener( Fastener( 0.*_mm, 0.*_mm, 4.83*_mm, 2.*_mm ) );


    Splice expected;
    expected.setAppliedLoad( Tensor( 10.*N, 0.*N, 0.*N_mm) );
    expected.addDesignSpace( ds );
    expected.addFastener( Fastener(   0.*_mm,  10.*_mm, 4.83*_mm, 2.*_mm ) );
    expected.addFastener( Fastener(   0.*_mm, -10.*_mm, 4.83*_mm, 2.*_mm ) );
    expected.addFastener( Fastener( -10.*_mm,   0.*_mm, 4.83*_mm, 2.*_mm ) );
    expected.addFastener( Fastener(  10.*_mm,   0.*_mm, 4.83*_mm, 2.*_mm ) );

    Splice actual;

    // When
    OptimisationSolver target;
    target.setSolver( &dummy );
    target.setDesignObjective( OptimisationDesignObjective::MinimizeMaxLoad );
    target.setDesignConstraints( OptimisationDesignConstraint::MinPitchDistance_4Phi );
    target.setRandomIterations( 100 );
    target.setInput(&input);
    target.setOutput(&actual);

    target.runSync();

    // Then
    SPLICE_COMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/

QTEST_APPLESS_MAIN(tst_OptimisationSolver)

#include "tst_optimisationsolver.moc"

