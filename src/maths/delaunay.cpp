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

#include "delaunay.h"

/* ************************************************************************** */
/* This header is a copy of the default Triangle's header                     */
/* (originally located in "3rd/triangle/triangle.h")                          */
/* with an ' extern "C" {} ' declaration to make it callable.                 */
/* ************************************************************************** */
/* BEGIN TRIANGLE DECLARATION */
#define VOID int
#ifdef SINGLE
#  define REAL float
#else /* not SINGLE */
#  define REAL double
#endif /* not SINGLE */
extern "C" {
#include "triangle.h"
}
/* END TRIANGLE DECLARATION */
/* ************************************************************************** */


#include <QtCore/QDebug>
#include <QtCore/QString>


namespace Maths
{

/* Reset all allocated arrays, including those allocated by Triangle. */
static void _q_initTriangulateIO(triangulateio &io)
{
    io.pointlist = Q_NULLPTR;
    io.pointattributelist = Q_NULLPTR;
    io.pointmarkerlist = Q_NULLPTR;
    io.numberofpoints = 0;
    io.numberofpointattributes = 0;
    io.trianglelist = Q_NULLPTR;
    io.triangleattributelist = Q_NULLPTR;
    io.trianglearealist = Q_NULLPTR;
    io.neighborlist = Q_NULLPTR;
    io.numberoftriangles = 0;
    io.numberofcorners = 0;
    io.numberoftriangleattributes = 0;
    io.segmentlist = Q_NULLPTR;
    io.segmentmarkerlist = Q_NULLPTR;
    io.numberofsegments = 0;
    io.holelist = Q_NULLPTR;
    io.numberofholes = 0;
    io.regionlist = Q_NULLPTR;
    io.numberofregions = 0;
    io.edgelist = Q_NULLPTR;
    io.edgemarkerlist = Q_NULLPTR;
    io.normlist = Q_NULLPTR;
    io.numberofedges = 0;
}

#define FREE_MEMBER(x) if (x) { free( x ); x = Q_NULLPTR; }

/* Free all allocated arrays, including those allocated by Triangle. */
static void _q_freeTriangulateIO(triangulateio &io)
{
    FREE_MEMBER( io.pointlist );
    FREE_MEMBER( io.pointattributelist );
    FREE_MEMBER( io.pointmarkerlist );
    io.numberofpoints = 0;
    io.numberofpointattributes = 0;
    FREE_MEMBER( io.trianglelist );
    FREE_MEMBER( io.triangleattributelist );
    FREE_MEMBER( io.trianglearealist );
    FREE_MEMBER( io.neighborlist );
    io.numberoftriangles = 0;
    io.numberofcorners = 0;
    io.numberoftriangleattributes = 0;
    FREE_MEMBER( io.segmentlist );
    FREE_MEMBER( io.segmentmarkerlist );
    io.numberofsegments = 0;
    FREE_MEMBER( io.holelist);
    io.numberofholes = 0;
    FREE_MEMBER( io.regionlist );
    io.numberofregions = 0;
    FREE_MEMBER( io.edgelist );
    FREE_MEMBER( io.edgemarkerlist );
    FREE_MEMBER( io.normlist );
    io.numberofedges = 0;
}

QList<QLineF> delaunayTriangulation(const QList<QPointF> &points)
{
    /***************************************\
    * Steps for Delaunay Triangulation:     *
    *                                       *
    *    - Sanitize the input               *
    *       - Remove duplicate points       *
    *       - Verify trivial results        *
    *       - Verify collinear points       *
    *    - Define input points              *
    *    - Triangulate the points           *
    *    - Convert the output points        *
    *    - Free the memory                  *
    *                                       *
    \***************************************/
    QList<QPointF> newpoints = points;
    for (int i = 0; i < newpoints.count()-1; ++i) {
        for (int j = i+1; j < newpoints.count(); ++j) {
            if (newpoints.at(i) == newpoints.at(j)) {
                newpoints.removeAt(j);
                --j;
            }
        }
    }

    if (newpoints.count() == 2) { /* trivial */
        QList<QLineF> res;
        res << QLineF(newpoints.at(0), newpoints.at(1));
        return res;
    }

    if (newpoints.count() < 3) {  /* Triangle needs at least 3 points */
        const QList<QLineF> empty;
        return empty;
    }


    {
        /**********************************************************\
        *        How to verify that 3 points are collinear?        *
        *                                                          *
        *   "*" = cross product, also called matrix determinant    *
        *                                                          *
        *   v1*v2 = det( [x1 x2] ) = x1*y2 − x2*y1                 *
        *              ( [y1 y2] )                                 *
        *                                                          *
        *   => v1*v2=0 implies v1 and v2 are collinear             *
        *                                                          *
        *   With 3 points:                                         *
        *                                                          *
        *   (p1−p0)*(p2−p0) = (x1−x0)*(y2−y0)−(x2−x0)*(y1−y0)      *
        *                   = 0 means p0, p1 and p2 are aligned    *
        *                                                          *
        \**********************************************************/
        Q_ASSERT(newpoints.count() > 2);

        const QPointF origin = newpoints.first();
        const QPointF v1 = newpoints.at(1) - origin;

        bool useX = (v1.x()!=0); /* otherwise use y */
        bool collinear = true;

        QList<qreal> sortedPointsPosition;
        QList<QPointF> sortedPoints;
        sortedPointsPosition << 0.;
        sortedPoints << newpoints.first();
        {
            qreal pos = (useX ? v1.x() : v1.y());
            if (pos > 0) {
                sortedPointsPosition << pos;
                sortedPoints << newpoints.at(1);
            } else {
                sortedPointsPosition.prepend(pos);
                sortedPoints.prepend(newpoints.at(1));
            }
        }

        QPointF v2;
        for (int i = 2; i < newpoints.count(); ++i) {
            v2 = newpoints.at(i) - origin;
            qreal det = v1.x()*v2.y() - v1.y()*v2.x();
            if (qFuzzyCompare(det, 0.0)) {
                bool inserted = false;
                qreal position = (useX ? v2.x() : v2.y());
                for (int j = 0; j < sortedPointsPosition.count(); ++j) {
                    Q_ASSERT(position != sortedPointsPosition.at(j));
                    if (position < sortedPointsPosition.at(j)) {
                        sortedPointsPosition.insert(j, position);
                        sortedPoints.insert(j, newpoints.at(i));
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    sortedPointsPosition << position;
                    sortedPoints << newpoints.at(i);
                }
            } else {
                collinear = false;
                break;
            }
        }
        if (collinear) {
            QList<QLineF> res;
            for (int i = 1; i < sortedPoints.count(); ++i) {
                res << QLineF(sortedPoints.at(i-1), sortedPoints.at(i));
            }
            return res;
        }
    }


    struct triangulateio in, out;
    _q_initTriangulateIO( in );
    _q_initTriangulateIO( out );

    /* Define input points. */
    in.numberofpoints = newpoints.count();
    in.numberofpointattributes = 0;
    in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
    for (int i = 0; i < newpoints.count(); ++i) {
        in.pointlist[ i * 2 ]     = newpoints.at(i).x();
        in.pointlist[ i * 2 + 1 ] = newpoints.at(i).y();
    }

    /******************************************************************\
    * triangulate()                                                    *
    *                                                                  *
    *  p = Read and write a Planar Straight Line Graph                 *
    *  c = Preserve the convex hull                                    *
    *  z = Number everything from zero (rather than one)               *
    *  e = Produce an edge list                                        *
    *  B = No boundary markers in the output                           *
    *  P = No output .poly file (saves disk space)                     *
    *  Q = Quiet, suppresses all messages except when error occurs     *
    *                                                                  *
    \******************************************************************/
    const QString option("pczeBPQ");
    triangulate(option.toLatin1().data(), &in, &out, Q_NULLPTR);

    QList<QLineF> res;
    for (int i = 0; i < out.numberofedges; ++i) {
        const int indexA = out.edgelist[ i * 2 ];
        const int indexB = out.edgelist[ i * 2 + 1 ];
        const QPointF pointA = newpoints.at(indexA);
        const QPointF pointB = newpoints.at(indexB);
        res << QLineF(pointA, pointB);
    }
    _q_freeTriangulateIO( in );
    _q_freeTriangulateIO( out );

    return res;
}

} // end namespace Maths
