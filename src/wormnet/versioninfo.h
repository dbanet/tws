/*
 * versioninfo.h
 *
 *  Created on: 04.11.2015
 *      Author: StepS
 */

#ifndef VERSIONINFO_H
#define VERSIONINFO_H
#include <QRegExp>
#include <QString>
#include <QVector>

#define MAX_VER_SIZE 32

class UVersion{
  private:
    QVector<quint16> words;
    unsigned errors;

  public:
    UVersion &operator=(QString s);
    UVersion &operator=(const UVersion &V);
    UVersion &operator=(const quint64 val64);
    UVersion &operator<<(const quint16 val);
    bool operator> (const QString &s) const;
    bool operator> (const UVersion &V) const;
    bool operator> (const quint64 val64) const;
    bool operator>=(const QString &s) const;
    bool operator>=(const UVersion &V) const;
    bool operator>=(const quint64 val64) const;
    bool operator< (const QString &s) const;
    bool operator< (const UVersion &V) const;
    bool operator< (const quint64 val64) const;
    bool operator<=(const QString &s) const;
    bool operator<=(const UVersion &V) const;
    bool operator<=(const quint64 val64) const;
    bool operator==(const QString &s) const;
    bool operator==(const UVersion &V) const;
    bool operator==(const quint64 val64) const;
    bool operator!=(const QString &s) const;
    bool operator!=(const UVersion &V) const;
    bool operator!=(const quint64 val64) const;
    quint16 &operator[](unsigned index);
    quint16 operator[](unsigned index) const;
    operator QString();
    operator quint64();

    bool is_between(const UVersion &V1, const UVersion &V2) const;
    bool is_between(const QString &s1, const QString &s2) const;
    bool is_valid() const;
    QString str() const;
    quint64 i64() const;
    quint16 num(unsigned index) const;
    void resize(unsigned size);
    unsigned size() const;
    UVersion(QString s);
    UVersion(quint64 val64);
    UVersion();
};
#endif // VERSIONINFO_H
