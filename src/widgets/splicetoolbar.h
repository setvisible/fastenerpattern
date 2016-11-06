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

#ifndef WIDGETS_SPLICE_TOOLBAR_H
#define WIDGETS_SPLICE_TOOLBAR_H

#include <QToolBar>
#include <Widgets/AbstractSpliceView>

class SpliceToolbar;
class SpliceToolbarPrivate :  public AbstractSpliceView
{
    Q_OBJECT
    friend class SpliceToolbar;
    SpliceToolbar* q_ptr;

public:
    explicit SpliceToolbarPrivate(QWidget *parent = 0);

public Q_SLOTS:
    virtual void selectionChanged() Q_DECL_OVERRIDE;

};

class SpliceToolbar : public QToolBar
{
    Q_OBJECT
    Q_PROPERTY(bool axesVisible READ isAxesVisible WRITE setAxesVisible NOTIFY axesVisibilityChanged)
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible NOTIFY gridVisibilityChanged)
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible NOTIFY imageVisibilityChanged)
    Q_PROPERTY(bool componentVisible READ isComponentVisible WRITE setComponentVisible NOTIFY componentVisibilityChanged)
    Q_PROPERTY(bool resultantVisible READ isResultantVisible WRITE setResultantVisible NOTIFY resultantVisibilityChanged)
    Q_PROPERTY(bool torqueVisible READ isTorqueVisible WRITE setTorqueVisible NOTIFY torqueVisibilityChanged)
    Q_PROPERTY(bool labelVisible READ isLabelVisible WRITE setLabelVisible NOTIFY labelVisibilityChanged)
    Q_PROPERTY(bool snapEnable READ isSnapEnable WRITE setSnapEnable NOTIFY snapEnabled)

    friend class SpliceToolbarPrivate;
    SpliceToolbarPrivate *d_ptr;

public:    
    explicit SpliceToolbar(const QString &title, QWidget *parent = Q_NULLPTR);
    explicit SpliceToolbar(QWidget *parent = Q_NULLPTR);
    virtual ~SpliceToolbar();

    void setModel(AbstractSpliceModel *model);
    AbstractSpliceModel *model() const;

Q_SIGNALS:
    void axesVisibilityChanged(bool visible);
    void gridVisibilityChanged(bool visible);
    void imageVisibilityChanged(bool visible);
    void componentVisibilityChanged(bool visible);
    void resultantVisibilityChanged(bool visible);
    void torqueVisibilityChanged(bool visible);
    void labelVisibilityChanged(bool visible);
    void snapEnabled(bool enabled);

public Q_SLOTS:
    void add();
    void duplicate();
    void selectAll();
    void remove();

    bool isAxesVisible() const;
    void setAxesVisible(bool visible);

    bool isGridVisible() const;
    void setGridVisible(bool visible);

    bool isImageVisible() const;
    void setImageVisible(bool visible);

    bool isComponentVisible() const;
    void setComponentVisible(bool visible);

    bool isResultantVisible() const;
    void setResultantVisible(bool visible);

    bool isTorqueVisible() const;
    void setTorqueVisible(bool visible);

    bool isLabelVisible() const;
    void setLabelVisible(bool visible);

    bool isSnapEnable() const;
    void setSnapEnable(bool enable);

protected:
    void _q_selectionChanged();

private:
    QAction *m_buttonAdd;
    QAction *m_buttonCopy;
    QAction *m_buttonSelectAll;
    QAction *m_buttonRemove;
    QAction *m_buttonShowAxes;
    QAction *m_buttonShowGrid;
    QAction *m_buttonShowBG;    /* BG = Background */
    QAction *m_buttonShowComponent;
    QAction *m_buttonShowResultant;
    QAction *m_buttonShowTorque;
    QAction *m_buttonShowLabel;
    QAction *m_buttonSnap;

    void createActions();
};

#endif // WIDGETS_SPLICE_TOOLBAR_H
