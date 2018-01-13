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

#ifndef CORE_CALCULATOR_H
#define CORE_CALCULATOR_H

#include <Core/SpliceCalculator>

#include <QtCore/QSet>

QT_BEGIN_NAMESPACE
class QString;
class QUndoStack;
QT_END_NAMESPACE


namespace SpliceCommand {
class SetTitle;
class SetAuthor;
class SetDate;
class SetDescription;
// --
class InsertFastener;
class SetFastener;
class RemoveFastener;
// --
class InsertDesignSpace;
class SetDesignSpace;
class RemoveDesignSpace;
// --
class SetAppliedLoad;
class SetSolverParameters;
}

class Calculator : public SpliceCalculator
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = Q_NULLPTR);

    QUndoStack *undoStack() const;

public Q_SLOTS:
    virtual void clear() Q_DECL_OVERRIDE;

    virtual void setTitle(const QString &title) Q_DECL_OVERRIDE;
    virtual void setAuthor(const QString &author) Q_DECL_OVERRIDE;
    virtual void setDate(const QString &date) Q_DECL_OVERRIDE;
    virtual void setDescription(const QString &description) Q_DECL_OVERRIDE;
    // --
    virtual void insertFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void setFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void removeFastener(const int index) Q_DECL_OVERRIDE;
    // --
    virtual void insertDesignSpace(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void setDesignSpace(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void removeDesignSpace(const int index) Q_DECL_OVERRIDE;
    // --
    virtual void setAppliedLoad(const Tensor &appliedLoad) Q_DECL_OVERRIDE;
    virtual void setSolverParameters(SolverParameters params) Q_DECL_OVERRIDE;

protected:
    /* Friend Classes */
    friend class SpliceCommand::SetTitle;
    friend class SpliceCommand::SetAuthor;
    friend class SpliceCommand::SetDate;
    friend class SpliceCommand::SetDescription;
    // --
    friend class SpliceCommand::InsertFastener;
    friend class SpliceCommand::SetFastener;
    friend class SpliceCommand::RemoveFastener;
    // --
    friend class SpliceCommand::InsertDesignSpace;
    friend class SpliceCommand::SetDesignSpace;
    friend class SpliceCommand::RemoveDesignSpace;
    // --
    friend class SpliceCommand::SetAppliedLoad;
    friend class SpliceCommand::SetSolverParameters;

    /* Callback Methods */
    void _q_setTitle(const QString &title);
    void _q_setAuthor(const QString &author);
    void _q_setDate(const QString &date);
    void _q_setDescription(const QString &description);
    // --
    void _q_insertFastener(const int index, const Fastener &fastener);
    void _q_setFastener(const int index, const Fastener &fastener);
    void _q_removeFastener(const int index);
    // --
    void _q_insertDesignSpace(const int index, const DesignSpace &designSpace);
    void _q_setDesignSpace(const int index, const DesignSpace &designSpace);
    void _q_removeDesignSpace(const int index);
    // --
    void _q_setAppliedLoad(const Tensor &appliedLoad);
    void _q_setSolverParameters(SolverParameters params);

private:
    QUndoStack* m_undoStack;

};

#endif // CORE_CALCULATOR_H
