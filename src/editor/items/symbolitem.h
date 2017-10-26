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

#ifndef EDITOR_ITEMS_SYMBOL_ITEM_H
#define EDITOR_ITEMS_SYMBOL_ITEM_H

#include <QtWidgets/QGraphicsItem>

class SymbolItem : public QGraphicsItem
{
    Q_PROPERTY(qreal diameter READ diameter WRITE setDiameter)

public:
    explicit SymbolItem(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) Q_DECL_OVERRIDE;

    qreal diameter() const;
    void setDiameter(qreal diameter);

private:
    qreal m_diameter;

};

#endif // EDITOR_ITEMS_SYMBOL_ITEM_H


/*
 wikipedia

 FASTENERS:
 =============

    anchor bolt
    batten
    bolt (fastener)
        Screw
    brass fastener
    buckle
    button
    cable tie
    captive fastener
    clamp (or cramp)
        hose clamp
    clasps
        lobster clasp
    cleko
    clips
        circlip
        hairpin clip
        paper clip
        terry clip
    clutch
    drawing pin (thumbtack)
    flange
    frog
    grommet
    hook-and-eye closure
    hook and loop fastener
        Velcro
    latch
    nail
    pegs
        clothespin
        tent peg
    PEM nut
    pins
        bowtie cotter pin
        circle cotter
        clevis fastener
        cotter
        dowel
        linchpin
        R-clip
        split pin
        spring pin
        tapered pin
    retaining rings
        circlip
        e-ring
    rivet
    rock bolt
    rubber band (or bands of other materials)
    screw anchor
    snap fastener
    staple
    stitches
    strap
    threaded fastener
        captive threaded fasteners
        nut
        screw
        washers
        threaded insert
        threaded rod
    tie
    toggle bolt
    treasury tag
    twist tie
    wedge anchor
    zipper


 */
