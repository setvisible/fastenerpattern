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

#ifndef EDITOR_SPLICE_GRAPHICS_WIDGET_H
#define EDITOR_SPLICE_GRAPHICS_WIDGET_H

#include <Widgets/AbstractSpliceView>
#include <Core/Fastener>
#include <Core/Tensor>

#include <QtCore/QUrl>

class QWidget;
class QVBoxLayout;
class BackgroundWidget;
class Splice;
class FastenerItem;
class TensorItem;
class SpliceGraphicsWidget : public AbstractSpliceView
{
    Q_OBJECT
    Q_PROPERTY(bool axesVisible READ isAxesVisible WRITE setAxesVisible)
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible)
    Q_PROPERTY(bool componentVisible READ isComponentVisible WRITE setComponentVisible)
    Q_PROPERTY(bool resultantVisible READ isResultantVisible WRITE setResultantVisible)
    Q_PROPERTY(bool torqueVisible READ isTorqueVisible WRITE setTorqueVisible)
    Q_PROPERTY(bool labelVisible READ isLabelVisible WRITE setLabelVisible)
    Q_PROPERTY(bool snapEnable READ isSnapEnable WRITE setSnapEnable)
    Q_PROPERTY(qreal pixelsPerUnit READ pixelsPerUnit WRITE setPixelsPerUnit)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl)

public:
    explicit SpliceGraphicsWidget(QWidget *parent = 0);
    virtual ~SpliceGraphicsWidget();

public Q_SLOTS:
    virtual void appliedLoadChanged() Q_DECL_OVERRIDE;
    virtual void selectionChanged() Q_DECL_OVERRIDE;
    virtual void resultsChanged() Q_DECL_OVERRIDE;
    virtual void fastenersInserted(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void fastenersChanged(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void fastenersRemoved(const int index) Q_DECL_OVERRIDE;

    /* Graphics Options */
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

    qreal pixelsPerUnit() const;
    void setPixelsPerUnit(qreal pxPerUnit);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

private Q_SLOTS:
    void update();
    void onSelectionChanged();
    void onFastenerPositionChanged();


private:
    QVBoxLayout *m_mainLayout;
    BackgroundWidget *m_backgroundWidget;
    TensorItem *m_appliedLoadItem;
    QList<FastenerItem*> m_fastenerItems;
    bool m_componentVisible;
    bool m_resultantVisible;
    bool m_torqueVisible;
    bool m_labelVisible;
    bool m_snapEnable;

    /// \todo  InstantaneousCentreOfRotation ( = + )
    /// \todo  Parts (Undeformed)
    /// \todo  Parts (Displaced)
    /// \todo  Fasteners (Displaced)
    /// \todo  FE Mesh
    /// \todo  Design Domain for Optimisation
    /// \todo  ...
};

#endif // EDITOR_SPLICE_GRAPHICS_WIDGET_H
