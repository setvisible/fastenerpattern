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
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    Q_PROPERTY(QString date READ date WRITE setDate)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(Tensor appliedLoad READ appliedLoad WRITE setAppliedLoad)

    //friend class SpliceCalculator;

public:
    explicit Splice(QObject *parent = 0);

    /* JSON Serialization */
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString title() const;
    void setTitle(const QString &title);

    QString author() const;
    void setAuthor(const QString &author);

    QString date() const;
    void setDate(const QString &date);

    QString description() const;
    void setDescription(const QString &description);

    Tensor appliedLoad() const;
    void setAppliedLoad(const Tensor &appliedLoad);


    int fastenerCount() const;
    const Fastener& fastenerAt(const int index) const;

    void insertFastener(const int index, const Fastener &fastener);
    void addFastener(const Fastener &fastener);
    void addFastener(const QList<Fastener> &fasteners);
    void setFastenerAt(const int index, const Fastener &fastener);
    void removeFastenerAt(const int index);
    void removeAllFasteners();

private:
//protected:
    QString m_title;
    QString m_author;
    QString m_date;
    QString m_description;

    Tensor m_appliedLoad;
    QList<Fastener> m_fasteners;

    /// \todo Solver Options
    /// \todo QPolygonF m_frontier;


};

#endif // CORE_SPLICE_H
