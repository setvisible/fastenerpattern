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

#ifndef CORE_SPLICE_H
#define CORE_SPLICE_H

#include <Core/Fastener>
#include <Core/Tensor>

#include <QObject>
#include <QString>
/// \todo #include <QPolygonF>

class QJsonObject;

class Splice : public QObject
{
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    Q_PROPERTY(Tensor appliedLoad READ appliedLoad WRITE setAppliedLoad)

    friend class SpliceCalculator;

public:
    explicit Splice(QObject *parent = 0);


    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString name() const;
    void setName(const QString &name);

    QString author() const;
    void setAuthor(const QString &author);

    Tensor appliedLoad() const;
    void setAppliedLoad(const Tensor &appliedLoad);

    QList<Fastener> fasteners() const;
    void addFastener(const Fastener &fastener);
    void addFastener(const QList<Fastener> &fasteners);
    void removeAllFasteners();

protected:
    QString m_name;
    QString m_author;

    Tensor m_appliedLoad;
    QList<Fastener> m_fasteners;

    /// \todo Solver Options
    /// \todo QPolygonF m_frontier;

private:

};

#endif // CORE_SPLICE_H
