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

#ifndef CORE_SPLICE_COMMAND_H
#define CORE_SPLICE_COMMAND_H

#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtWidgets/QUndoCommand>

#include <Core/Calculator>

#define C_COMMAND_ID_SET_TITLE             10
#define C_COMMAND_ID_SET_AUTHOR            11
#define C_COMMAND_ID_SET_DATE              12
#define C_COMMAND_ID_SET_DESCRIPTION       13
#define C_COMMAND_ID_SET_FASTENER          20
#define C_COMMAND_ID_SET_DESIGN_SPACE      30
#define C_COMMAND_ID_SET_APPLIED_LOAD      40


namespace SpliceCommand {

class SetTitle : public QUndoCommand
{
public:
    SetTitle(Calculator *calc, const QString &title, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_title(title), m_previous(m_calc->title())
    { this->setText("Change Title"); }
    virtual void undo() { m_calc->_q_setTitle(m_previous); }
    virtual void redo() { m_calc->_q_setTitle(m_title);    }
    virtual int id() const { return C_COMMAND_ID_SET_TITLE; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        m_title = static_cast<const SetTitle*>(other)->m_title;
        return true;
    }
private:
    Calculator *m_calc;
    QString m_title;
    QString m_previous;
};


class SetAuthor : public QUndoCommand
{
public:
    SetAuthor(Calculator *calc, const QString &author, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_author(author), m_previous(m_calc->author())
    { this->setText("Change Author"); }
    virtual void undo() { m_calc->_q_setAuthor(m_previous); }
    virtual void redo() { m_calc->_q_setAuthor(m_author); }
    virtual int id() const { return C_COMMAND_ID_SET_AUTHOR; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        m_author = static_cast<const SetAuthor*>(other)->m_author;
        return true;
    }
private:
    Calculator *m_calc;
    QString m_author;
    QString m_previous;
};

class SetDate : public QUndoCommand
{
public:
    SetDate(Calculator *calc, const QString &date, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_date(date), m_previous(m_calc->date())
    { this->setText("Change Date"); }
    virtual void undo() { m_calc->_q_setDate(m_previous); }
    virtual void redo() { m_calc->_q_setDate(m_date); }
    virtual int id() const { return C_COMMAND_ID_SET_DATE; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        m_date = static_cast<const SetDate*>(other)->m_date;
        return true;
    }
private:
    Calculator *m_calc;
    QString m_date;
    QString m_previous;
};


class SetDescription : public QUndoCommand
{
public:
    SetDescription(Calculator *calc, const QString &description, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_description(description), m_previous(m_calc->description())
    { this->setText("Change Description"); }
    virtual void undo() { m_calc->_q_setDescription(m_previous); }
    virtual void redo() { m_calc->_q_setDescription(m_description); }
    virtual int id() const { return C_COMMAND_ID_SET_DESCRIPTION; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        m_description = static_cast<const SetDescription*>(other)->m_description;
        return true;
    }
private:
    Calculator *m_calc;
    QString m_description;
    QString m_previous;
};

/******************************************************************************
 ******************************************************************************/

///  \todo add class ModifyGroup: public QUndoCommand


/******************************************************************************
 ******************************************************************************/
class InsertFastener : public QUndoCommand
{
public:
    InsertFastener(Calculator *calc, int index, Fastener fastener, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_index(index), m_fastener(fastener)
    { this->setText("Insert Fastener"); }
    virtual void undo() { m_calc->_q_removeFastener(m_index); }
    virtual void redo() { m_calc->_q_insertFastener(m_index, m_fastener); }
private:
    Calculator *m_calc;
    int m_index;
    Fastener m_fastener;
};

class SetFastener : public QUndoCommand
{
public:
    SetFastener(Calculator *calc, int index, Fastener fastener, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc)
    {
        this->setText("Modify Fastener");
        m_fasteners.insert(index, fastener);
        m_previous.insert(index, m_calc->fastenerAt(index));
    }
    virtual void undo()
    {
        QMapIterator<int, Fastener> i(m_previous);
        while (i.hasNext()) {
            i.next();
            m_calc->_q_setFastener(i.key(), i.value());
        }
    }
    virtual void redo()
    {
        QMapIterator<int, Fastener> i(m_fasteners);
        while (i.hasNext()) {
            i.next();
            m_calc->_q_setFastener(i.key(), i.value());
        }
    }
    virtual int id() const { return C_COMMAND_ID_SET_FASTENER; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        QMap<int, Fastener> previous = static_cast<const SetFastener*>(other)->m_previous;
        QMapIterator<int, Fastener> prev(previous);
        while (prev.hasNext()) {
            prev.next();
            if (!m_previous.contains(prev.key())) {
                m_previous.insert(prev.key(), prev.value());
            }
        }
        QMap<int, Fastener> fasteners = static_cast<const SetFastener*>(other)->m_fasteners;
        QMapIterator<int, Fastener> i(fasteners);
        while (i.hasNext()) {
            i.next();
            m_fasteners.insert(i.key(), i.value());
        }
        return true;
    }
private:
    Calculator *m_calc;
    QMap<int, Fastener> m_previous;
    QMap<int, Fastener> m_fasteners;
};

class RemoveFastener : public QUndoCommand
{
public:
    RemoveFastener(Calculator *calc, int index, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_index(index), m_previous(m_calc->fastenerAt(index))
    { this->setText("Remove Fastener"); }
    virtual void undo() { m_calc->_q_insertFastener(m_index, m_previous); }
    virtual void redo() { m_calc->_q_removeFastener(m_index); }
private:
    Calculator *m_calc;
    int m_index;
    Fastener m_previous;
};



/******************************************************************************
 ******************************************************************************/
class InsertDesignSpace : public QUndoCommand
{
public:
    InsertDesignSpace(Calculator *calc, int index, DesignSpace designSpace, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_index(index), m_designSpace(designSpace)
    { this->setText("Insert Design Space"); }
    virtual void undo() { m_calc->_q_removeDesignSpace(m_index); }
    virtual void redo() { m_calc->_q_insertDesignSpace(m_index, m_designSpace); }
private:
    Calculator *m_calc;
    int m_index;
    DesignSpace m_designSpace;
};

class SetDesignSpace : public QUndoCommand
{
public:
    SetDesignSpace(Calculator *calc, int index, DesignSpace designSpace, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc)
    {
        this->setText("Modify Design Space");
        m_designSpaces.insert(index, designSpace);
        m_previous.insert(index, m_calc->designSpaceAt(index));
    }
    virtual void undo()
    {
        QMapIterator<int, DesignSpace> i(m_previous);
        while (i.hasNext()) {
            i.next();
            m_calc->_q_setDesignSpace(i.key(), i.value());
        }
    }
    virtual void redo()
    {
        QMapIterator<int, DesignSpace> i(m_designSpaces);
        while (i.hasNext()) {
            i.next();
            m_calc->_q_setDesignSpace(i.key(), i.value());
        }
    }
    virtual int id() const { return C_COMMAND_ID_SET_DESIGN_SPACE; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        QMap<int, DesignSpace> previous = static_cast<const SetDesignSpace*>(other)->m_previous;
        QMapIterator<int, DesignSpace> prev(previous);
        while (prev.hasNext()) {
            prev.next();
            if (!m_previous.contains(prev.key())) {
                m_previous.insert(prev.key(), prev.value());
            }
        }
        QMap<int, DesignSpace> designSpaces = static_cast<const SetDesignSpace*>(other)->m_designSpaces;
        QMapIterator<int, DesignSpace> i(designSpaces);
        while (i.hasNext()) {
            i.next();
            m_designSpaces.insert(i.key(), i.value());
        }
        return true;
    }
private:
    Calculator *m_calc;
    QMap<int, DesignSpace> m_previous;
    QMap<int, DesignSpace> m_designSpaces;
};

class RemoveDesignSpace : public QUndoCommand
{
public:
    RemoveDesignSpace(Calculator *calc, int index, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_index(index), m_previous(m_calc->designSpaceAt(index))
    { this->setText("Remove Design Space"); }
    virtual void undo() { m_calc->_q_insertDesignSpace(m_index, m_previous); }
    virtual void redo() { m_calc->_q_removeDesignSpace(m_index); }
private:
    Calculator *m_calc;
    int m_index;
    DesignSpace m_previous;
};


/******************************************************************************
 ******************************************************************************/
class SetAppliedLoad : public QUndoCommand
{
public:
    SetAppliedLoad(Calculator *calc, const Tensor &appliedLoad, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_load(appliedLoad), m_previous(m_calc->appliedLoad())
    { this->setText("Change Applied Load"); }
    virtual void undo() { m_calc->_q_setAppliedLoad(m_previous); }
    virtual void redo() { m_calc->_q_setAppliedLoad(m_load); }
    virtual int id() const { return C_COMMAND_ID_SET_APPLIED_LOAD; }
    bool mergeWith(const QUndoCommand *other)
    {
        if (other->id() != id())
            return false;
        m_load = static_cast<const SetAppliedLoad*>(other)->m_load;
        return true;
    }
private:
    Calculator *m_calc;
    Tensor m_load;
    Tensor m_previous;
};

class SetSolverParameters : public QUndoCommand
{
public:
    SetSolverParameters(Calculator *calc, SolverParameters params, QUndoCommand *parent = Q_NULLPTR)
        : QUndoCommand(parent), m_calc(calc), m_params(params), m_previous(m_calc->solverParameters())
    { this->setText("Change Solver Parameters"); }
    virtual void undo() { m_calc->_q_setSolverParameters(m_previous); }
    virtual void redo() { m_calc->_q_setSolverParameters(m_params); }
private:
    Calculator *m_calc;
    SolverParameters m_params;
    SolverParameters m_previous;
};


} // namespace SpliceCommand

#endif // CORE_SPLICE_COMMAND_H
