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

#include "abstractsplicemodel.h"

#include <QtCore/QSet>

/*! \class AbstractSpliceModel
 *  \brief The class AbstractSpliceModel is a model (MVC paradigm).
 *
 * It synchronize the data and selections for the views, that must derived
 * from the class AbstractSpliceView.
 *
 * For instance, when the user clicks on a fastener in the GraphicsView,
 * the other widgets in the GUI (for instance, the Table widget with the
 * list of the fasteners, the "Property Widget" that displays the properties
 * of the selected fastener, and the Fastener's ToolBar related to the
 * fastener's state) are automatically updated accordingly to the selection.
 *
 * This is an elegant way to reduce the responsibility of the MainWindow class,
 * because the MainWindow is no more in charge of connecting the
 * GUI's signals & slots.
 *
 * \sa AbstractSpliceView.
 */

