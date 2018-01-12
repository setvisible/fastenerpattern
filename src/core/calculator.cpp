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

#include "calculator.h"

#include <Core/SpliceCommand>

#include <QtWidgets/QUndoStack>


/*! \class Calculator
 *  \brief The class Calculator is an adapter class for SpliceCalculator.
 *
 * It manages the Undo/Redo Mechanism for the SpliceCalculator.
 */

Calculator::Calculator(QObject *parent) : SpliceCalculator(parent)
  , m_undoStack(new QUndoStack(this))
{
    this->clear();
}

QUndoStack* Calculator::undoStack() const
{
    return m_undoStack;
}

/******************************************************************************
 ******************************************************************************/
void Calculator::clear()
{
    SpliceCalculator::clear();
    m_undoStack->clear();
}

/******************************************************************************
 ******************************************************************************/
void Calculator::setTitle(const QString &title)
{
    if (SpliceCalculator::title() != title) {
        m_undoStack->push(new SpliceCommand::SetTitle(this, title));
    }
}

void Calculator::setAuthor(const QString &author)
{
    if (SpliceCalculator::author() != author) {
        m_undoStack->push(new SpliceCommand::SetAuthor(this, author));
    }
}

void Calculator::setDate(const QString &date)
{
    if (SpliceCalculator::date() != date) {
        m_undoStack->push(new SpliceCommand::SetDate(this, date));
    }
}

void Calculator::setDescription(const QString &description)
{
    if (SpliceCalculator::description() != description) {
        m_undoStack->push(new SpliceCommand::SetDescription(this, description));
    }
}

// -----------------------------------------------------------------------------
void Calculator::insertFastener(const int index, const Fastener &fastener)
{
    m_undoStack->push(new SpliceCommand::InsertFastener(this, index, fastener));
}

void Calculator::setFastener(const int index, const Fastener &fastener)
{
    m_undoStack->push(new SpliceCommand::SetFastener(this, index, fastener));
}

void Calculator::removeFastener(const int index)
{
    m_undoStack->push(new SpliceCommand::RemoveFastener(this, index));
}

// -----------------------------------------------------------------------------
void Calculator::insertDesignSpace(const int index, const DesignSpace &designSpace)
{
    m_undoStack->push(new SpliceCommand::InsertDesignSpace(this, index, designSpace));
}

void Calculator::setDesignSpace(const int index, const DesignSpace &designSpace)
{
    m_undoStack->push(new SpliceCommand::SetDesignSpace(this, index, designSpace));
}

void Calculator::removeDesignSpace(const int index)
{
    m_undoStack->push(new SpliceCommand::RemoveDesignSpace(this, index));
}

// -----------------------------------------------------------------------------
void Calculator::setAppliedLoad(const Tensor &appliedLoad)
{
    if (SpliceCalculator::appliedLoad() != appliedLoad) {
        m_undoStack->push(new SpliceCommand::SetAppliedLoad(this, appliedLoad));
    }
}

void Calculator::setSolverParameters(SolverParameters params)
{
    if (SpliceCalculator::solverParameters() != params) {
        m_undoStack->push(new SpliceCommand::SetSolverParameters(this, params));
    }
}

/******************************************************************************
 ******************************************************************************/
/* Callback Methods */
void Calculator::_q_setTitle(const QString &title)
{
    SpliceCalculator::setTitle(title);
}

void Calculator::_q_setAuthor(const QString &author)
{
    SpliceCalculator::setAuthor(author);
}

void Calculator::_q_setDate(const QString &date)
{
    SpliceCalculator::setDate(date);
}

void Calculator::_q_setDescription(const QString &description)
{
    SpliceCalculator::setDescription(description);
}

// -----------------------------------------------------------------------------
void Calculator::_q_insertFastener(const int index, const Fastener &fastener)
{
    SpliceCalculator::insertFastener(index, fastener);
}

void Calculator::_q_setFastener(const int index, const Fastener &fastener)
{
    SpliceCalculator::setFastener(index, fastener);
}

void Calculator::_q_removeFastener(const int index)
{
    SpliceCalculator::removeFastener(index);
}

// -----------------------------------------------------------------------------
void Calculator::_q_insertDesignSpace(const int index, const DesignSpace &designSpace)
{
    SpliceCalculator::insertDesignSpace(index, designSpace);
}

void Calculator::_q_setDesignSpace(const int index, const DesignSpace &designSpace)
{
    SpliceCalculator::setDesignSpace(index, designSpace);
}

void Calculator::_q_removeDesignSpace(const int index)
{
    SpliceCalculator::removeDesignSpace(index);
}

// -----------------------------------------------------------------------------
void Calculator::_q_setAppliedLoad(const Tensor &appliedLoad)
{
    SpliceCalculator::setAppliedLoad(appliedLoad);
}

void Calculator::_q_setSolverParameters(SolverParameters params)
{
    SpliceCalculator::setSolverParameters(params);
}

