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

#ifndef EDITOR_ITEMS_SCALE_H
#define EDITOR_ITEMS_SCALE_H

/* ************************************************************** */
/* Here, we define some common constants for consistant graphics  */
/* ************************************************************** */

/* Define the default DPI for the screen. */
#define C_DEFAULT_SCREEN_DPI  10.0      /* --> 1 mm = 10 pixels */


/* Background Grid */
#define C_BACKGROUND_PEN_WIDTH        1 /*  pixel */
#define C_BACKGROUND_MIN_UNIT_SIZE    5
#define C_BACKGROUND_MINOR_LINES     10


/* Fastener Symbol */
/* NB: values are expressed in percent of the fastener diameter. */
#define C_SYMBOL_PERCENT_PEN_WIDTH         0.01
#define C_SYMBOL_PERCENT_AXES_LENGTH       1.50
#define C_SYMBOL_PERCENT_HOLE_DIAMETER     0.50
#define C_SYMBOL_PERCENT_THREAD_DIAMETER   0.40


/* Arrows */
#define C_ARROW_SIZE 10      /* pixels */


#endif // EDITOR_ITEMS_SCALE_H
