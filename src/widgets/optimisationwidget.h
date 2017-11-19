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

#ifndef WIDGETS_OPTIMISATION_WIDGET_H
#define WIDGETS_OPTIMISATION_WIDGET_H

#include <QtCore/QThread>
#include <QtWidgets/QWidget>

#include <Core/Optimizer/Controller>
#include <Core/Optimizer/OptimisationSolver>

class Splice;
class SpliceCalculator;
class OptimisationSolver;

namespace Ui {
class OptimisationWidget;
}

/// \todo class OptimisationWidget : public AbstractSpliceView ?

class OptimisationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptimisationWidget(QWidget *parent = Q_NULLPTR);
    virtual ~OptimisationWidget();

    SpliceCalculator *spliceCalculator() const;
    void setSpliceCalculator(SpliceCalculator *calculator);

private Q_SLOTS:
    void onControllerStarted();
    void onControllerProgressed(int percent);
    void onControllerStopped();
    void onControllerMessageInfo(qint64 timestamp, QString message);
    void onControllerMessageWarning(qint64 timestamp, QString message);
    void onControllerMessageFatal(qint64 timestamp, QString message);
    void onControllerMessageDebug(QString message);

    void onShowResultToggled(bool checked);

    void start();
    void stop();

private:
    Ui::OptimisationWidget *ui;
    Controller *m_controller;
    SpliceCalculator *m_calculator;

    void log(int type, qint64 timestamp, const QString &message);
};

#endif // WIDGETS_OPTIMISATION_WIDGET_H
