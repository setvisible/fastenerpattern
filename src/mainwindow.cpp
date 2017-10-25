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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "version.h"
#include <Core/SpliceCalculator>
#include <Dialogs/PropertiesDialog>
#include <Widgets/AppliedLoadWidget>
#include <Widgets/DesignObjectiveWidget>
#include <Widgets/DesignOptionWidget>
#include <Widgets/DesignSpaceWidget>
#include <Widgets/DesignVariableWidget>
#include <Widgets/FastenerWidget>
#include <Widgets/MainWidget>
#include <Widgets/OptimisationWidget>
#include <Widgets/ResultWidget>
#include <Widgets/SolverWidget>
#include <Widgets/TableWidget>


#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_calculator(new SpliceCalculator(this))
  , m_dirty(false)
  , m_physicalFile(false)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,10);


    ui->spliceToolBar->setModel(m_calculator);
    ui->spliceGraphicsWidget->setModel(m_calculator);
    ui->mainWidget->fastenerWidget()->setModel(m_calculator);
    ui->mainWidget->appliedLoadWidget()->setModel(m_calculator);
    ui->mainWidget->designObjectiveWidget()->setModel(m_calculator);
    ui->mainWidget->designOptionWidget()->setModel(m_calculator);
    ui->mainWidget->designSpaceWidget()->setModel(m_calculator);
    ui->mainWidget->designVariableWidget()->setModel(m_calculator);
    ui->mainWidget->optimisationWidget()->setModel(m_calculator);
    ui->mainWidget->resultWidget()->setModel(m_calculator);
    ui->mainWidget->tableWidget()->setModel(m_calculator);

    /* [1] */
    /* Connect the GUI to the Calculator. */
    QObject::connect(ui->action_Add, SIGNAL(triggered(bool)),
                     ui->spliceToolBar, SLOT(add()));
    QObject::connect(ui->action_Duplicate, SIGNAL(triggered(bool)),
                     ui->spliceToolBar, SLOT(duplicate()));
    QObject::connect(ui->action_Remove, SIGNAL(triggered(bool)),
                     ui->spliceToolBar, SLOT(remove()));
    QObject::connect(ui->action_SelectAll, SIGNAL(triggered(bool)),
                     ui->spliceToolBar, SLOT(selectAll()));

    QObject::connect(ui->mainWidget->solverWidget(), SIGNAL(paramsChanged(SolverParameters)),
                     m_calculator, SLOT(setSolverParameters(SolverParameters)));

    m_calculator->setSolverParameters(ui->mainWidget->solverWidget()->params());


    /* [2] */
    /* Connect the Calculator to the MainWindow. */
    /* In fact, the Calculator centralizes the changes. */
    QObject::connect(m_calculator, SIGNAL(changed()), this, SLOT(setDirty()));

    /* [3] */
    /* Internally, the Calculator updates the Splice, and
     * recalculates the results thanks to the Solvers.
     * It emits the signal changed() to inform the GUI.
     */


    /* [4] */
    /* Connect the Calculator to the GUI. */
    /* Update the GUI with the new results. */


    /* [5] */
    /* Connect the rest of the GUI widgets together (selection, focus, etc.) */
    QObject::connect(ui->spliceToolBar, SIGNAL(axesVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setAxesVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(gridVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setGridVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(imageVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setImageVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(componentVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setComponentVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(resultantVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setResultantVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(torqueVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setTorqueVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(labelVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setLabelVisible(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(snapEnabled(bool)),
                     ui->spliceGraphicsWidget, SLOT(setSnapEnable(bool)));
    QObject::connect(ui->spliceToolBar, SIGNAL(distanceVisibilityChanged(bool)),
                     ui->spliceGraphicsWidget, SLOT(setDistanceVisible(bool)));


    createActions();
    createMenus();

    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        /// \todo XXX store settings ?
        event->accept();
    } else {
        event->ignore();
    }
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::newFile()
{
    if (maybeSave()) {
        m_physicalFile = false;
        m_currentFile = QFileInfo();
        m_currentFile.setFile(QStringLiteral("untitled.splice"));
        m_calculator->clear();

        /* Settings */
        ui->spliceToolBar->setAxesVisible(true);
        ui->spliceToolBar->setGridVisible(true);
        ui->spliceToolBar->setImageVisible(false);
        ui->spliceGraphicsWidget->setImageUrl(QUrl());
        ui->spliceToolBar->setComponentVisible(false);
        ui->spliceToolBar->setResultantVisible(true);
        ui->spliceToolBar->setTorqueVisible(true);
        ui->spliceToolBar->setLabelVisible(false);
        ui->spliceToolBar->setSnapEnable(false);

        setClean();
    }
}

bool MainWindow::isExampleFile() const
{
    return m_currentFile.filePath().startsWith(":/");
}

bool MainWindow::isPhysicalFile() const
{
    return m_physicalFile
            && m_currentFile.isFile()
            && !m_currentFile.isSymLink()
            && !isExampleFile();
}

bool MainWindow::save()
{
    if ( isPhysicalFile() ){
        return saveFile(m_currentFile.absoluteFilePath());
    } else {
        return saveAs();
    }
}

bool MainWindow::saveAs()
{
    QString filePath = askSaveFileName(QStringLiteral("Splice Data File (*.splice)"),
                                       tr("Splice Data File"));
    if (filePath.isEmpty()) {
        return false;
    }
    return saveFile(filePath);
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString filePath = askOpenFileName(tr("Splice Data File (*.splice);;All files (*.*)"));
        if (!filePath.isEmpty()) {
            if (loadFile(filePath)) {
                this->setClean();
            }
        }
    }
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::showFileProperties()
{
    PropertiesDialog dialog(m_calculator, this);
    dialog.exec();
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::about()
{
    QMessageBox msgBox(QMessageBox::NoIcon, tr("About FastenerPattern"), aboutHtml());
    msgBox.exec();
}

/***********************************************************************************
 ***********************************************************************************/
bool MainWindow::maybeSave()
{
    if (m_dirty) {
        int ret = QMessageBox::warning(
                    this, tr("Unsaved changes"),
                    tr("The splice has unsaved changes.\n\n"
                       "Do you want to save your changes ?"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return save();
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

QString MainWindow::niceFileName() const
{
    if ( isPhysicalFile() ){
        return QDir::toNativeSeparators(m_currentFile.canonicalFilePath());
    }else{
        return m_currentFile.fileName();
    }
}

void MainWindow::setDirty()
{
    if (!m_dirty) {
        m_dirty = true;
        this->setWindowTitle( niceFileName() + QStringLiteral("* - FastenerPattern"));
    }
}

void MainWindow::setClean()
{
    m_dirty = false;
    this->setWindowTitle( niceFileName() + QStringLiteral(" - FastenerPattern "));
}

/***********************************************************************************
 ***********************************************************************************/
QString MainWindow::askSaveFileName(const QString &fileFilter, const QString &title)
{
    QString suggestedPath;
    if ( isPhysicalFile() ) {
        suggestedPath = m_currentFile.canonicalFilePath();
    }else{
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        suggestedPath = dir + QDir::separator() + m_currentFile.fileName();
        suggestedPath = QDir::toNativeSeparators(suggestedPath);
    }
    return QFileDialog::getSaveFileName(this, title, suggestedPath, fileFilter);
}

QString MainWindow::askOpenFileName(const QString &fileFilter, const QString &title)
{
    QString currentDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if ( isPhysicalFile() ){
        currentDir = m_currentFile.absolutePath();
    }
    return QFileDialog::getOpenFileName(this, title, currentDir, fileFilter);
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::createActions()
{
    ui->action_New->setShortcuts(QKeySequence::New);
    ui->action_New->setStatusTip(tr("Create a new splice"));
    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    ui->action_Open->setShortcuts(QKeySequence::Open);
    ui->action_Open->setStatusTip(tr("Open a splice from file"));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));

    ui->action_Save->setShortcuts(QKeySequence::Save);
    ui->action_Save->setStatusTip(tr("Save the splice to current file"));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));

    ui->action_SaveAs->setShortcuts(QKeySequence::SaveAs);
    ui->action_SaveAs->setStatusTip(tr("Save the splice to a different file"));
    connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

#ifdef Q_OS_WIN
    ui->action_Properties->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F10));
#else
    ui->action_Properties->setShortcuts(QKeySequence::Preferences);
#endif
    ui->action_Properties->setStatusTip(tr("Show the properties of the current file"));
    connect(ui->action_Properties, SIGNAL(triggered()), this, SLOT(showFileProperties()));

    ui->action_Exit->setShortcuts(QKeySequence::Quit);
    ui->action_Exit->setStatusTip(tr("Quit FastenerPattern"));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));

    ui->action_About->setShortcuts(QKeySequence::HelpContents);
    ui->action_About->setStatusTip(tr("About FastenerPattern"));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));

    ui->action_AboutQt->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F1));
    ui->action_AboutQt->setStatusTip(tr("About Qt"));
    connect(ui->action_AboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{

}

/***********************************************************************************
 ***********************************************************************************/

bool MainWindow::saveFile(const QString &path)
{
    QDir::setCurrent(path);
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        QMessageBox::warning(this, tr("Cannot save file"),
                             tr("Cannot write to file %1:\n%2.")
                             .arg(path)
                             .arg(file.errorString()));
        return false;
    }

    QJsonObject json;
    m_calculator->write(json);
    QJsonDocument saveDoc(json);
    file.write( saveDoc.toJson() );

    m_physicalFile = true;
    m_currentFile.setFile(path);
    this->statusBar()->showMessage(tr("File saved"), 2000);
    setClean();
    return true;
}


/***********************************************************************************
 ***********************************************************************************/
bool MainWindow::loadFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open file.");
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(path)
                             .arg(file.errorString()));
        return false;
    }
    QByteArray saveData = file.readAll();
    QJsonParseError ok;
    QJsonDocument loadDoc( QJsonDocument::fromJson(saveData, &ok) );

    if (ok.error != QJsonParseError::NoError) {
        qCritical("Couldn't parse JSON file.");
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot parse the JSON file:\n"
                                "%1\n\n"
                                "At character %2, %3.\n\n"
                                "Operation cancelled.")
                             .arg(path)
                             .arg(ok.offset)
                             .arg(ok.errorString()));
        return false;
    }

    m_calculator->read(loadDoc.object());
    m_physicalFile = true;
    m_currentFile = path;
    this->statusBar()->showMessage(tr("File loaded"), 5000);
    setClean();
    return true;
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::on_action_4BoltJoint_triggered()
{
    loadFile(":/examples/4BoltJoint.splice");
}

void MainWindow::on_action_PatternJoint_triggered()
{
    loadFile(":/examples/PatternJoint.splice");
}

void MainWindow::on_action_RandomPattern_triggered()
{
    loadFile(":/examples/RandomJoint.splice");
}
