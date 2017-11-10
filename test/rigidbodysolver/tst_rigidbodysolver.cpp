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

#include <Core/Solvers/RigidBodySolver>

#include <QtTest/QtTest>
#include <QtCore/QDebug>
#include <QtCore/QList>

#include <iostream>

using namespace boost;
using namespace units;
using namespace si;


class tst_RigidBodySolver : public QObject
{
    Q_OBJECT

private slots:
    void test_empty_splice();
    void test_undefined_load();
    void test_undefined_fasteners();

    void test_simple();
    void test_moment();
    void test_different_degrees_of_freedom();

    void test_isobearing();
    void test_isoshear();

};


/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_empty_splice()
{
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 0);  /* No results */
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_undefined_load()
{
    /* Here we forget to define an external load ;) No load is applied ;) */
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.addFastener( Fastener(  0.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );
    splice.addFastener( Fastener( 20.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 2);
    QCOMPARE( actual.at(0), Tensor() );  /* No results */
    QCOMPARE( actual.at(1), Tensor() );
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_undefined_fasteners()
{
    /* Here we forget to define fixations */
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1205.*N, 0.*N, 0.*N_m) );
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 0); /* No results */
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_simple()
{
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1205.*N, 0.*N, 0.*N_m ) );
    splice.addFastener( Fastener(  0.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );
    splice.addFastener( Fastener( 20.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );

    /* Rem: the mode doesn't matter here (same bolts = same load sharing) */
    solver.setParameters(SolverParameters::RigidBodySolverWithIsoBearing);

    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 2);
    QCOMPARE( actual.at(0).around(), Tensor(602.50*N, -60.25*N, 0.*N_m) );
    QCOMPARE( actual.at(1).around(), Tensor(602.50*N,  60.25*N, 0.*N_m) );
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_moment()
{
    /* With non-null moment */
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 0.*N, 100000.*N_mm) );
    splice.addFastener( Fastener(  0.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );
    splice.addFastener( Fastener( 20.*_mm, 1.*_mm, 4.83*_mm, 3.*_mm ) );
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 2);
    QCOMPARE( actual.at(0), Tensor(500.*N, -5050.*N, 0.*N_m) );
    QCOMPARE( actual.at(1), Tensor(500.*N,  5050.*N, 0.*N_m) );
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_different_degrees_of_freedom()
{
    /* With different Degrees-Of-Freedom (DoF) */
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 0.*N, 0.*N_mm) );
    splice.addFastener( Fastener(  0.*_mm,  1.*_mm, 4.83*_mm, 3.*_mm, Fastener::Free , Fastener::Fixed));
    splice.addFastener( Fastener( 20.*_mm,  1.*_mm, 4.83*_mm, 3.*_mm, Fastener::Fixed, Fastener::Fixed));
    splice.addFastener( Fastener( 10.*_mm, 10.*_mm, 4.83*_mm, 3.*_mm, Fastener::Fixed, Fastener::Free ));
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 3);
    QCOMPARE( actual.at(0).around(2), Tensor(   0.00*N, -228.69*N, 0.*N_m) );
    QCOMPARE( actual.at(1).around(2), Tensor( 602.91*N,  228.69*N, 0.*N_m) );
    QCOMPARE( actual.at(2).around(2), Tensor( 397.09*N,    0.00*N, 0.*N_m) );
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_isobearing()
{
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    splice.addFastener( Fastener( -10.*_mm, -5.*_mm, 6.45*_mm, 3.*_mm ) );
    splice.addFastener( Fastener(  10.*_mm, -5.*_mm, 4.83*_mm, 2.*_mm ) );
    splice.addFastener( Fastener(   0.*_mm, 10.*_mm, 2.20*_mm, 1.*_mm ) );

    solver.setParameters(SolverParameters::RigidBodySolverWithIsoBearing);/* IsoBearing */
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 3);
    QCOMPARE( actual.at(0).around(2), Tensor( 621.08*N, 612.93*N, 0.*N_m ) );
    QCOMPARE( actual.at(1).around(2), Tensor( 310.06*N, 316.22*N, 0.*N_m ) );
    QCOMPARE( actual.at(2).around(2), Tensor(  68.87*N,  70.85*N, 0.*N_m ) );
}

/******************************************************************************
 ******************************************************************************/
void tst_RigidBodySolver::test_isoshear()
{
    // Given, When
    RigidBodySolver solver;
    Splice splice;
    splice.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
    splice.addFastener( Fastener( -10.*_mm, -5.*_mm, 6.45*_mm, 3.*_mm ) );
    splice.addFastener( Fastener(  10.*_mm, -5.*_mm, 4.83*_mm, 2.*_mm ) );
    splice.addFastener( Fastener(   0.*_mm, 10.*_mm, 2.20*_mm, 1.*_mm ) );

    solver.setParameters(SolverParameters::RigidBodySolverWithIsoShear); /* IsoShear */
    QList<Tensor> actual = solver.calculate( &splice );

    // Then
    QCOMPARE( actual.count(), 3);
    QCOMPARE( actual.at(0).around(2), Tensor( 594.17*N, 611.14*N, 0.*N_m ) );
    QCOMPARE( actual.at(1).around(2), Tensor( 333.19*N, 320.10*N, 0.*N_m ) );
    QCOMPARE( actual.at(2).around(2), Tensor(  72.64*N,  68.76*N, 0.*N_m ) );
}

QTEST_APPLESS_MAIN(tst_RigidBodySolver)

#include "tst_rigidbodysolver.moc"

