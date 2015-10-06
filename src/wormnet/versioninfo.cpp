/*
 * versioninfo.cpp
 *
 *  Created on: 04.11.2015
 *      Author: StepS
 */

#include <QRegExp>
#include <QString>
#include <QVector>
#include "versioninfo.h"

unsigned UVersion::size() const{
    return words.size();
}

void UVersion::resize(unsigned size){
    if (size <= MAX_VER_SIZE)
        words.resize(size);
}

QString UVersion::str() const{
    QString result = "";
    if (size() > 0){
        result = QString::number(num(0));
        for (unsigned i = 1; i < size(); i ++){
            result += "." + QString::number(num(i));
            if (i >= MAX_VER_SIZE-1)
                break;
        }
    }
    return result;
}

quint64 UVersion::i64() const{
    return (quint64(num(0)) << 48) | (quint64(num(1)) << 32) | (quint64(num(2)) << 16) | (quint64(num(3)));
}

quint16 UVersion::num(unsigned index) const{
    if (index < size())
        return words[index];
    return 0;
}

bool UVersion::is_between(const UVersion &V1, const UVersion &V2) const{
    unsigned maxsize = size();
    if (V1.size() > maxsize)
        maxsize = V1.size();
    if (V2.size() > maxsize)
        maxsize = V2.size();

    for (unsigned i = 0; i < maxsize; i++){
        if (num(i) < V1.num(i))
            break;
        else if (num(i) > V2.num(i))
            break;
        else if (num(i) > V1.num(i) && num(i) < V2.num(i))
            return true;
    }
    return false;
}

bool UVersion::is_between(const QString &s1, const QString &s2) const{
    return is_between(UVersion(s1), UVersion(s2));
}

bool UVersion::is_valid() const{
    return !errors;
}

UVersion &UVersion::operator=(QString s){
    errors = 0;
    resize(0);
    if (s != ""){
        s += ".";
        while (s.indexOf(".") != -1){
            QString sect = s.section('.', 0, 0);
            sect.remove(QRegExp("[^0-9]"));
            if (sect == "")
                errors++;
            unsigned V = sect.toInt();
            resize(size()+1);
            words[size()-1] = V;

            if (size() >= MAX_VER_SIZE)
                break;

            s = s.section('.', 1, -1);
        }
    }
    else
        errors++;
    return *this;
}

UVersion &UVersion::operator=(const UVersion &V){
    operator=(V.str());
    return *this;
}

UVersion &UVersion::operator=(const quint64 val64){
    resize(4);
    words[0] = quint16(val64 >> 48);
    words[1] = quint16(val64 >> 32);
    words[2] = quint16(val64 >> 16);
    words[3] = quint16(val64);
    return *this;
}

UVersion &UVersion::operator<<(const quint16 val){
    resize(size()+1);
    words[size()-1] = val;
    return *this;
}

bool UVersion::operator>(const UVersion &V) const{
    for (unsigned i = 0; i < (V.size() > size() ? V.size() : size()); i++){
        if (num(i) > V.num(i))
            return true;
        else if (num(i) < V.num(i))
            break;
    }
    return false;
}

bool UVersion::operator<(const UVersion &V) const{
    for (unsigned i = 0; i < (V.size() > size() ? V.size() : size()); i++){
        if (num(i) < V.num(i))
            return true;
        else if (num(i) > V.num(i))
            break;
    }
    return false;
}

bool UVersion::operator>(const quint64 val64) const{
    return i64() > val64;
}

bool UVersion::operator>(const QString &s) const{
    return operator>(UVersion(s));
}

bool UVersion::operator<(const QString &s) const{
    return operator<(UVersion(s));
}
bool UVersion::operator<(const quint64 val64) const{
    return i64() < val64;
}

bool UVersion::operator>=(const QString &s) const{
    return !operator<(s);
}

bool UVersion::operator>=(const UVersion &V) const{
    return !operator<(V);
}

bool UVersion::operator>=(const quint64 val64) const{
    return i64() >= val64;
}

bool UVersion::operator<=(const QString &s) const{
    return !operator>(s);
}

bool UVersion::operator<=(const UVersion &V) const{
    return !operator>(V);
}

bool UVersion::operator<=(const quint64 val64) const{
    return i64() <= val64;
}

bool UVersion::operator==(const QString &s) const{
    return operator==(UVersion(s));
}

bool UVersion::operator==(const UVersion &V) const{
    for (unsigned i = 0; i < (V.size() > size() ? V.size() : size()); i++)
        if (num(i) != V.num(i))
            return false;
    return true;
}

bool UVersion::operator==(const quint64 val64) const{
    return i64() == val64;
}

bool UVersion::operator!=(const QString &s) const{
    return !operator==(s);
}

bool UVersion::operator!=(const UVersion &V) const{
    return !operator==(V);
}

bool UVersion::operator!=(const quint64 val64) const{
    return i64() != val64;
}

quint16 &UVersion::operator[](unsigned index){
    return words[index];
}

quint16 UVersion::operator[](unsigned index) const{
    return num(index);
}

UVersion::operator QString(){
    return str();
}
UVersion::operator quint64(){
    return i64();
}

UVersion::UVersion(QString s){
    operator=(s);
}

UVersion::UVersion(quint64 val64){
    operator=(val64);
}

UVersion::UVersion(){
    resize(0);
}
