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

#ifndef CORE_SPLICE_CALCULATOR_H
#define CORE_SPLICE_CALCULATOR_H

#include <Core/AbstractSpliceModel>

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

QT_BEGIN_NAMESPACE
class QDebug;
class QJsonObject;
class QObject;
class QString;
QT_END_NAMESPACE

class ISolver;
class Splice;
enum class SolverParameters;

class SpliceCalculator : public AbstractSpliceModel
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    Q_PROPERTY(QString date READ date WRITE setDate)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(SolverParameters solverParameters READ solverParameters WRITE setSolverParameters)

public:
    explicit SpliceCalculator(QObject *parent = Q_NULLPTR);

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString title() const;
    QString author() const;
    QString date() const;
    QString description() const;

    virtual int fastenerCount() const Q_DECL_OVERRIDE;
    virtual Fastener fastenerAt(const int index) const Q_DECL_OVERRIDE;

    virtual int designSpaceCount() const Q_DECL_OVERRIDE;
    virtual DesignSpace designSpaceAt(const int index) const Q_DECL_OVERRIDE;

    virtual Tensor appliedLoad() const Q_DECL_OVERRIDE;
    virtual Tensor resultAt(const int index) const Q_DECL_OVERRIDE;
    virtual SolverParameters solverParameters() const Q_DECL_OVERRIDE;

    virtual QSet<int> selectedFastenerIndexes() const Q_DECL_OVERRIDE;
    virtual QSet<int> selectedDesignSpaceIndexes() const Q_DECL_OVERRIDE;

    virtual ISolver* solver() const Q_DECL_OVERRIDE { return m_solver; }

Q_SIGNALS:
    void changed();

public Q_SLOTS:
    virtual void clear();

    virtual void setTitle(const QString &title);
    virtual void setAuthor(const QString &author);
    virtual void setDate(const QString &date);
    virtual void setDescription(const QString &description);

    virtual void insertFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void setFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual void removeFastener(const int index) Q_DECL_OVERRIDE;

    virtual void insertDesignSpace(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void setDesignSpace(const int index, const DesignSpace &designSpace) Q_DECL_OVERRIDE;
    virtual void removeDesignSpace(const int index) Q_DECL_OVERRIDE;

    virtual void setAppliedLoad(const Tensor &appliedLoad) Q_DECL_OVERRIDE;
    virtual void setSolverParameters(SolverParameters params) Q_DECL_OVERRIDE;

    virtual void setFastenerSelection(const QSet<int> indexes) Q_DECL_OVERRIDE;
    virtual void setDesignSpaceSelection(const QSet<int> indexes) Q_DECL_OVERRIDE;

private:
    SolverParameters m_params;
    ISolver *m_solver;
    QSharedPointer<Splice> m_splice;
    QSet<int> m_selectedFastenerIndexes;
    QSet<int> m_selectedDesignSpaceIndexes;
    QList<Tensor> m_results;

    void recalculate();
};

#endif // CORE_SPLICE_CALCULATOR_H
