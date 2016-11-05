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

#ifndef ABOUT_H
#define ABOUT_H

#include <QCoreApplication>
#include "version.h"

QString buildAbout(QString paragraphDelimiter)
{
    return QString(QCoreApplication::tr("FastenerPattern version ") + QStringLiteral(APP_VERSION) +
                   paragraphDelimiter +
                   QCoreApplication::tr( "FastenerPattern is a solution for designing "
                                         "fastener patterns (i.e. bolted and riveted joints).") +
                   paragraphDelimiter +
                   QCoreApplication::tr(
                       "FastenerPattern is licensed under the GNU LGPL version 3.1. "
                       "In Short this means that you can use and distribute "
                       "the FastenerPattern binaries without restrictions.") +
                   paragraphDelimiter +
                   QCoreApplication::tr(
                       "The majority of the FastenerPattern code is licensed under the "
                       "GNU LGPL version 3 (Lesser General Public License). "
                       "Meaning that you can reuse these parts of the code under the "
                       "conditions of the GNU LGPL version 3 license. "
                       "However, since some source files are licensed under the GNU LGPL "
                       "version 3, the entire application inherits this license.") +
                   paragraphDelimiter +
                   QCoreApplication::tr("Copyright 2016 Sébastien Vavassori. All rights reserved."));
}

QString about()
{
    return buildAbout("\n\n");
}

QString aboutHtml()
{
    return QString("<h3>" + QCoreApplication::tr("About FastenerPattern") + "</h3>" +
                   "<p><img src=\":/icons/logo/logo_128x128.png\" /></p>" +
                   "<p></p>" +
                   "<p>" + buildAbout("</p><p>") + "</p>");
}

#endif // ABOUT_H
