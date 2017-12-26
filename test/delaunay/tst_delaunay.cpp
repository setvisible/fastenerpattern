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

#include <Math/Delaunay>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

class tst_Delaunay : public QObject
{
    Q_OBJECT

private slots:
    void test_empty();
    void test_one_point();
    void test_coincident_points();
    void test_coincident_points_2();
    void test_coincident_points_3();

    void test_two_points();
    void test_three_points();

    void test_collinear_points();
    void test_collinear_points_along_x();
    void test_collinear_points_along_y();
    void test_collinear_points_along_y_2();
    void test_collinear_points_along_xy();
};


/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_empty()
{
    // Given, When
    QList<QPointF> points;
    QList<QLineF> expected;
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_one_point()
{
    // Given
    QList<QPointF> points;
    points << QPointF(10,20);

    QList<QLineF> expected;

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_coincident_points()
{
    // Given
    QList<QPointF> points;
    points << QPointF(10,20) << QPointF(10,20);

    QList<QLineF> expected;

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_coincident_points_2()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(0,0) << QPointF(0,0) << QPointF(0,0);

    QList<QLineF> expected;

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}


/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_coincident_points_3()
{
    // Given
    QList<QPointF> points;
    points << QPointF(10,20) << QPointF(10,20) << QPointF(5,0);

    QList<QLineF> expected;
    expected << QLineF(10,20,5,0);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_two_points()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(10,0);

    QList<QLineF> expected;
    expected << QLineF(0,0,10,0);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_three_points()
{
    // Given
    QList<QPointF> points;
    points << QPointF(-10,0) << QPointF(10,0) << QPointF(0,10);

    QList<QLineF> expected;
    expected << QLineF(-10,0,10,0) << QLineF(10,0,0,10) << QLineF(0,10,-10,0);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_collinear_points()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(10,0) << QPointF(20,0); // collinear

    QList<QLineF> expected;
    expected << QLineF(0,0,10,0) << QLineF(10,0,20,0);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_collinear_points_along_x()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(10,0) << QPointF(-10,0); // collinear

    QList<QLineF> expected;
    expected << QLineF(-10,0,0,0) << QLineF(0,0,10,0);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_collinear_points_along_y()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(0,10) << QPointF(0,-10); // collinear

    QList<QLineF> expected;
    expected << QLineF(0,-10,0,0) << QLineF(0,0,0,10);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_collinear_points_along_y_2()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(0,-10) << QPointF(0,10); // collinear

    QList<QLineF> expected;
    expected << QLineF(0,-10,0,0) << QLineF(0,0,0,10);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_Delaunay::test_collinear_points_along_xy()
{
    // Given
    QList<QPointF> points;
    points << QPointF(0,0) << QPointF(10,-2) << QPointF(-10,2); // collinear

    QList<QLineF> expected;
    expected << QLineF(-10,2,0,0) << QLineF(0,0,10,-2);

    // When
    QList<QLineF> actual = Math::delaunayTriangulation(points);

    // Then
    QCOMPARE(actual, expected);
}

QTEST_APPLESS_MAIN(tst_Delaunay)

#include "tst_delaunay.moc"
