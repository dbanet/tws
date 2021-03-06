#ifndef WA_TEXTCODEC_H
#define WA_TEXTCODEC_H

#include <QTextCodec>
#include <QHash>
class WA_textcodec1252 : public QTextCodec
{
public:
    QByteArray name () const;
    QList<QByteArray> aliases () const;
    int mibEnum () const;
    explicit WA_textcodec1252();
    ~WA_textcodec1252();    
protected:
    QString convertToUnicode ( const char * chars, int len, ConverterState *) const;
    QByteArray convertFromUnicode ( const QChar * input, int number, ConverterState *) const;
private:
    QHash<char,QChar> hashtounicode;
    QHash<QChar,char> hashfromunicode;


};
class WA_textcodec1251 : public QTextCodec
{
public:
    QByteArray name () const;
    QList<QByteArray> aliases () const;
    int mibEnum () const;
    WA_textcodec1251();
    ~WA_textcodec1251();
protected:
    QString convertToUnicode ( const char * chars, int len, ConverterState *) const;
    QByteArray convertFromUnicode ( const QChar * input, int number, ConverterState *) const;
private:
    QHash<char,QChar> hashtounicode;
    QHash<QChar,char> hashfromunicode;
};
#endif // WA_TEXTCODEC_H
