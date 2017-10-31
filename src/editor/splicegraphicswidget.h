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

#ifndef EDITOR_SPLICE_GRAPHICS_WIDGET_H
#define EDITOR_SPLICE_GRAPHICS_WIDGET_H

#include <Widgets/AbstractSpliceView>
#include <Core/Fastener>
#include <Core/Tensor>

#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE
class QWidget;
class QVBoxLayout;
QT_END_NAMESPACE

class BackgroundWidget;
class DesignSpaceItem;
class FastenerItem;
class MeasureItem;
class Splice;
class TensorItem;

class SpliceGraphicsWidget : public AbstractSpliceView
{
    Q_OBJECT
public:
    explicit SpliceGraphicsWidget(QWidget *parent = 0);
    virtual ~SpliceGraphicsWidget();

public Q_SLOTS:
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

    bool isDistanceVisible() const;
    void setDistanceVisible(bool visible);

    qreal pixelsPerUnit() const;
    void setPixelsPerUnit(qreal pxPerUnit);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    bool isDesignSpaceVisible() const;
    void setDesignSpaceVisible(bool visible);


public Q_SLOTS:
    virtual void onFastenerInserted(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void onFastenerChanged(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void onFastenerRemoved(const int index) Q_DECL_OVERRIDE;

    virtual void onDesignSpaceInserted(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void onDesignSpaceChanged(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void onDesignSpaceRemoved(const int index) Q_DECL_OVERRIDE;

    virtual void onAppliedLoadChanged() Q_DECL_OVERRIDE;
    virtual void onResultsChanged() Q_DECL_OVERRIDE;

    virtual void onSelectionFastenerChanged() Q_DECL_OVERRIDE;
    virtual void onSelectionDesignSpaceChanged() Q_DECL_OVERRIDE;


private Q_SLOTS:
    void update();
    void onSelectionChanged();
    void onFastenerItemPositionChanged();
    void onDesignSpaceItemChanged();

private:
    QVBoxLayout *m_mainLayout;
    BackgroundWidget *m_backgroundWidget;
    TensorItem *m_appliedLoadItem;
    QList<FastenerItem*> m_fastenerItems;
    QList<DesignSpaceItem*> m_designSpaceItems;
    QList<MeasureItem*> m_measureItems;

    bool m_componentVisible;
    bool m_resultantVisible;
    bool m_torqueVisible;
    bool m_labelVisible;
    bool m_snapEnable;           /* Magnetize objects onto the grid */
    bool m_distanceVisible;
    bool m_designSpaceVisible;

    /// \todo  InstantaneousCentreOfRotation ( = + )
    /// \todo  Parts (Undeformed)
    /// \todo  Parts (Displaced)
    /// \todo  Fasteners (Displaced)
    /// \todo  FE Mesh
    /// \todo  Design Domain for Optimisation
    /// \todo  ...

    void updateDistanceItemPositions();

};

#endif // EDITOR_SPLICE_GRAPHICS_WIDGET_H
