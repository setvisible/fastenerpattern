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

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <QtCore/QtGlobal> /* qFuzzyCompare() */
#include <QtCore/QtMath>   /* qsrand(), qPow() */
#include <time.h>

namespace Math {

namespace Utils {

/******************************************************************************
 ******************************************************************************/
Q_DECL_CONSTEXPR static inline bool fuzzyCompare(double p1, double p2)
{
    return qFuzzyCompare(p1, p2);
}

Q_DECL_CONSTEXPR static inline bool fuzzyCompare(float p1, float p2)
{
    return qFuzzyCompare(p1, p2);
}

/******************************************************************************
 ******************************************************************************/
static bool seeded = false;

/*!
 * \brief Return a random value between 0.0 and 1.0 (included).
 */
static inline qreal rand()
{
    if (!seeded) {
        /* Seed random number generator */
        qsrand((unsigned) time(0));
        seeded = true;
    }
    Q_ASSERT(RAND_MAX > 0);
    return (qreal)(qrand()) / RAND_MAX;
}

/******************************************************************************
 ******************************************************************************/
static inline qreal round(qreal f, int precision)
{
    if (precision < 0) precision = 0;
    if (precision > 9) precision = 9;
    qreal significantDigits = qPow(10, precision);
    qreal round = qFloor((f * significantDigits) + 0.5) / significantDigits;
    return round;
}

/******************************************************************************
 ******************************************************************************/

} // end namespace Utils

} // end namespace Math

#endif // MATH_UTILS_H
