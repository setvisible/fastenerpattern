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

#include <QtTest/QtTest>
#include <QtCore/QDebug>

class tst_OptimisationSolver : public QObject
{
    Q_OBJECT

private slots:
    void test_trivial_case_1();

};


/*************************************************************************
 *************************************************************************/
void tst_OptimisationSolver::test_trivial_case_1()
{
   // // Given
   // Splice splice;
   // splice.setAppliedLoad( Tensor( 1000.*N, 1000.*N, 1000.*N_mm) );
   // splice.addFastener( Fastener( -10.*_mm, -5.*_mm, 6.45*_mm, 3.*_mm ) );
   // splice.addFastener( Fastener(  10.*_mm, -5.*_mm, 4.83*_mm, 2.*_mm ) );
   // splice.addFastener( Fastener(   0.*_mm, 10.*_mm, 2.20*_mm, 1.*_mm ) );
   //
   // OptimisationDomain domain;
   // domain.appendPolygonalRegion(QRectF(0.,0.,10.,10.));
   // QCOMPARE( domain.regionCount(), 1);
   // domain.associate(splice.fastenerAt(0), domain.regionAt(0));
   // domain.associate(splice.fastenerAt(1), domain.regionAt(0));
   // domain.associate(splice.fastenerAt(2), domain.regionAt(0));

  //  QSharedPointer<DummySolver> solver(new DummySolver());

    OptimisationSolver target;
  //  target.setDesignVariable( splice );
   // target.setDesignConstraints( domain );
    target.setDesignObjective( SolverParameters::RigidBodySolverWithIsoBearing );
  //  target.setDesignParameters( 0.1, 1000 ); // discretization, max iteration, ...

    // When

    target.run();
   // OptimisationResult actual = target.run();
   //
   // // Then
   // QVERIFY( actual.isValid() ); // has no error: ex: 1 solution reached. no 'invalid' region
   // QVERIFY( actual.bestSolution() > 149.*N );
   // QVERIFY( actual.bestSolution() < 150.*N );
   //
   // Splice os = actual.optimizedSplice();
   // QCOMPARE( os.fastenerAt(0), Fastener(   0.*_mm, -5.*_mm, 6.45*_mm, 3.*_mm ) );
   // QCOMPARE( os.fastenerAt(1), Fastener(  10.*_mm, -5.*_mm, 4.83*_mm, 2.*_mm ) );
   // QCOMPARE( os.fastenerAt(2), Fastener(   0.*_mm, 10.*_mm, 2.20*_mm, 1.*_mm ) );


}


/*************************************************************************
 *************************************************************************/

QTEST_APPLESS_MAIN(tst_OptimisationSolver)

#include "tst_optimisationsolver.moc"

