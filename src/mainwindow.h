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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QFileInfo>
#include <QtWidgets/QMainWindow>

class Calculator;

class QUndoView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool saveFile(const QString &path);
    bool loadFile(const QString &path);

protected:
    void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    void newFile();
    bool save();
    bool saveAs();
    void open();
    void showFileProperties();
    void showUndoRedoPanel(bool toggled);

    void about();

    bool maybeSave();

    void setDirty();
    void setClean();

    void setUndoText(const QString &undoText);
    void setRedoText(const QString &redoText);

    void on_action_4BoltJoint_triggered();
    void on_action_PatternJoint_triggered();
    void on_action_RandomPattern_triggered();
    void on_action_Optimize4Bolt_triggered();


private:
    Ui::MainWindow *ui;
    Calculator *m_calculator;
    QUndoView *m_undoRedoPanel;

    bool m_dirty;
    bool m_physicalFile;
    QFileInfo m_currentFile;

    inline QString niceFileName() const;
    inline bool isExampleFile() const;
    inline bool isPhysicalFile() const;

    void createActions();
    void createMenus();

    QString askSaveFileName(const QString &fileFilter, const QString &title = tr("Save As"));
    QString askOpenFileName(const QString &fileFilter, const QString &title = tr("Open"));

};

#endif // MAINWINDOW_H
