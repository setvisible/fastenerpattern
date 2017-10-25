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
    // Remove duplicate points
    QList<QPointF> newpoints = points;
    for (int i = 0; i < newpoints.count()-1; ++i) {
        for (int j = i+1; j < newpoints.count(); ++j) {
            if (newpoints.at(i) == newpoints.at(j)) {
                newpoints.removeAt(j);
            }
        }
    }
    qDebug() << Q_FUNC_INFO << newpoints;

    if (newpoints.count() == 2) { // trivial
        QList<QLineF> res;
        res << QLineF(newpoints.at(0), newpoints.at(1));
        return res;
    }

    if (newpoints.count() < 3) {  // Triangle needs at least 3 points
        QList<QLineF> empty;
        return empty;
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

    /* Triangle Switches */
    QString option("pczeBP");
    QLatin1String quiet("Q");
    /*  p = Read and write a Planar Straight Line Graph
     *  c = Preserve the convex hull
     *  z = Number everything from zero (rather than one)
     *  e = Produce an edge list
     *  B = No boundary markers in the output
     *  P = No output .poly file. Saves disk space
     *  Q = Quiet. Suppresses all Triangle messages except when error occurs
     */

    /* Triangulate the points. */
#ifdef QT_DEBUG
    Q_UNUSED(quiet);
    triangulate(option.toLatin1().data(), &in, &out, Q_NULLPTR);
#else
    option.append(quiet);
    triangulate(option.toLatin1().data(), &in, &out, Q_NULLPTR);
#endif

    QList<QLineF> res;
    for (int i = 0; i < out.numberofedges; ++i) {
        const int indexA = out.edgelist[ i * 2 ];
        const int indexB = out.edgelist[ i * 2 + 1 ];
        QPointF pointA = newpoints.at(indexA);
        QPointF pointB = newpoints.at(indexB);
        res << QLineF(pointA, pointB);
    }
    _q_freeTriangulateIO( in );
    _q_freeTriangulateIO( out );

    return res;
}

} // end namespace Maths
