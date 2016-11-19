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

#ifndef CORE_SPLICE_CALCULATOR_H
#define CORE_SPLICE_CALCULATOR_H

#include <Core/AbstractSpliceModel>
#include <Core/Splice>
#include <Core/Solvers/Parameters>

#include <QObject>
#include <QString>

class QJsonObject;

class ISolver;
class Splice;
class SpliceCalculator : public AbstractSpliceModel
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    Q_PROPERTY(QString date READ date WRITE setDate)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(SolverParameters solverParameters READ solverParameters WRITE setSolverParameters)

public:
    explicit SpliceCalculator(QObject *parent = 0);

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void clear();

    QString title() const;
    void setTitle(const QString &title);

    QString author() const;
    void setAuthor(const QString &author);

    QString date() const;
    void setDate(const QString &date);

    QString description() const;
    void setDescription(const QString &description);

    virtual int fastenerCount() const Q_DECL_OVERRIDE;
    virtual Fastener fastenerAt(const int index) const Q_DECL_OVERRIDE;
    virtual Tensor appliedLoad() const Q_DECL_OVERRIDE;
    virtual Tensor resultAt(const int index) const Q_DECL_OVERRIDE;
    virtual QSet<int> selectedIndexes() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void changed();

public Q_SLOTS:
    virtual bool insertFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual bool removeFastener(const int index) Q_DECL_OVERRIDE;
    virtual bool setFastener(const int index, const Fastener &fastener) Q_DECL_OVERRIDE;
    virtual bool setAppliedLoad(const Tensor &appliedLoad) Q_DECL_OVERRIDE;
    virtual bool setSelection(const QSet<int> indexes) Q_DECL_OVERRIDE;

    SolverParameters solverParameters() const;
    void setSolverParameters(SolverParameters params);

private:
    SolverParameters m_params;
    ISolver *m_solver;
    Splice* m_splice;
    QSet<int> m_selectedIndexes;
    QList<Tensor> m_results;

    void recalculate();

};

#endif // CORE_SPLICE_CALCULATOR_H
