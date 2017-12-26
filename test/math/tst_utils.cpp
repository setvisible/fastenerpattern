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

#include <Math/Utils>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

class tst_Utils : public QObject
{
    Q_OBJECT

private slots:
    void test_uniformely_distributed_random_data();
    void test_uniformely_distributed_random();

};

void tst_Utils::test_uniformely_distributed_random_data()
{
    QTest::addColumn<int>("shoots");
    QTest::addColumn<int>("precision");
    QTest::newRow("") << 10 << 10;        // 10 shoots upon 10 intervals
    QTest::newRow("") << 1000 << 10;      // 1000 shoots upon 10 intervals
    QTest::newRow("") << 1000 << 100;     // 1000 shoots upon 100 intervals
    QTest::newRow("") << 100000 << 100;   // -etc.-
    QTest::newRow("") << 10000 << 1000;
    QTest::newRow("") << 100000 << 1000;
}

/*!
 * This test checks that rand() is uniformely distributed.
 * That is, the probability to get any number between 0 and 1 is the same.
 */
void tst_Utils::test_uniformely_distributed_random()
{
    // Given
    QFETCH(int, shoots);    // total of dice tries
    QFETCH(int, precision); // intervals between 0. and 1.

    /*
     * Here we use a magic number 10 to calculate the 'acceptable' divergence.
     * This comes from different experiments that shows that the distribution
     * of random numbers is more and more precise when the number of shoots
     * (dice tries) grows.
     */
    const qreal maxEpsilonAllowable = 10.0 * (qreal)precision / (qreal)shoots;

    QVector<int> results;
    results.resize( precision );
    for (int i = 0; i < precision; ++i) {
        QVERIFY( results.at(i) == 0 );
    }

    // When
    const int count = precision * shoots;
    for (int i = 0; i < count; ++i) {
        qreal x = Math::Utils::rand(); /* between 0.0 and 1.0 */
        int index = qFloor( x * precision );
        if (index >= precision) {
            index = precision - 1;
        }
        results[index]++;
    }

    // Then
    qreal maxEpsilon = 0.0;
    for (int i = 0; i < precision; ++i) {
        qreal epsilon = 1.0 - (qreal)results.at(i) / (qreal)shoots;
        maxEpsilon = qMax(maxEpsilon, qAbs(epsilon));
    }
    QString details =
            QString("(max allowed = %0%) actual = %1%.")
            .arg(100. * maxEpsilonAllowable)
            .arg(100. * maxEpsilon);
    QString description =
            QString("The quality requirement has not be found: %0").arg(details);
    QVERIFY2( maxEpsilon <= maxEpsilonAllowable, description.toLatin1().data() );
    qDebug() << details;
}

QTEST_APPLESS_MAIN(tst_Utils)

#include "tst_utils.moc"
