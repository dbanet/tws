#include "WA_textcodec.h"

#include<QObject>
#include<QDebug>
#include<QHash>

QHash<char,QChar> hashtounicode;
QHash<QChar,char> hashfromunicode;

WA_textcodec1252::WA_textcodec1252(){
    hashtounicode[0x41]=L'A';
    hashfromunicode[L'А']=0x41;
    hashtounicode[0x42]=L'B';
    hashfromunicode[L'В']=0x42;
    hashtounicode[0x43]=L'C';
    hashfromunicode[L'С']=0x43;
    hashtounicode[0x45]=L'E';
    hashfromunicode[L'Е']=0x45;
    hashtounicode[0x48]=L'H';
    hashfromunicode[L'Н']=0x48;
    hashtounicode[0x4D]=L'M';
    hashfromunicode[L'М']=0x4D;
    hashtounicode[0x4F]=L'O';
    hashfromunicode[L'О']=0x4F;
    hashtounicode[0x50]=L'P';
    hashfromunicode[L'Р']=0x50;
    hashtounicode[0x54]=L'T';
    hashfromunicode[L'Т']=0x54;
    hashtounicode[0x58]=L'X';
    hashfromunicode[L'Х']=0x58;
    hashtounicode[0x61]=L'a';
    hashfromunicode[L'а']=0x61;
    hashtounicode[0x63]=L'c';
    hashfromunicode[L'с']=0x63;
    hashtounicode[0x65]=L'e';
    hashfromunicode[L'е']=0x65;
    hashtounicode[0x6F]=L'o';
    hashfromunicode[L'о']=0x6F;
    hashtounicode[0x70]=L'p';
    hashfromunicode[L'р']=0x70;
    hashtounicode[0x78]=L'x';
    hashfromunicode[L'х']=0x78;
    hashtounicode[0x79]=L'y';
    hashfromunicode[L'у']=0x79;
    hashtounicode[0xCB]=L'Ë';
    hashfromunicode[L'Ё']=0xCB;
    hashtounicode[0xEB]=L'ë';
    hashfromunicode[L'ё']=0xEB;
    hashtounicode[0x80]=L'Б';
    hashfromunicode[L'Б']=0x80;
    hashtounicode[0x81]=L'Г';
    hashfromunicode[L'Г']=0x81;
    hashtounicode[0x82]=L'Д';
    hashfromunicode[L'Д']=0x82;
    hashtounicode[0x83]=L'Ж';
    hashfromunicode[L'Ж']=0x83;
    hashtounicode[0x84]=L'З';
    hashfromunicode[L'З']=0x84;
    hashtounicode[0x85]=L'И';
    hashfromunicode[L'И']=0x85;
    hashtounicode[0x86]=L'Й';
    hashfromunicode[L'Й']=0x86;
    hashtounicode[0x87]=L'К';
    hashfromunicode[L'К']=0x87;
    hashtounicode[0x88]=L'Л';
    hashfromunicode[L'Л']=0x88;
    hashtounicode[0x89]=L'П';
    hashfromunicode[L'П']=0x89;
    hashtounicode[0x8A]=L'У';
    hashfromunicode[L'У']=0x8A;
    hashtounicode[0x8B]=L'Ф';
    hashfromunicode[L'Ф']=0x8B;
    hashtounicode[0x8C]=L'Ц';
    hashfromunicode[L'Ц']=0x8C;
    hashtounicode[0x8D]=L'Ч';
    hashfromunicode[L'Ч']=0x8D;
    hashtounicode[0x8E]=L'Щ';
    hashfromunicode[L'Ш']=0x8E;
    hashtounicode[0x8F]=L'Щ';
    hashfromunicode[L'Щ']=0x8F;
    hashtounicode[0x90]=L'Ъ';
    hashfromunicode[L'Ъ']=0x90;
    hashtounicode[0x91]=L'Ы';
    hashfromunicode[L'Ы']=0x91;
    hashtounicode[0x92]=L'Ь';
    hashfromunicode[L'Ь']=0x92;
    hashtounicode[0x93]=L'Э';
    hashfromunicode[L'Э']=0x93;
    hashtounicode[0x94]=L'Ю';
    hashfromunicode[L'Ю']=0x94;
    hashtounicode[0x96]=L'Я';
    hashfromunicode[L'Я']=0x96;
    hashtounicode[0x97]=L'б';
    hashfromunicode[L'б']=0x97;
    hashtounicode[0x98]=L'в';
    hashfromunicode[L'в']=0x98;
    hashtounicode[0x99]=L'г';
    hashfromunicode[L'г']=0x99;
    hashtounicode[0x9A]=L'д';
    hashfromunicode[L'д']=0x9A;
    hashtounicode[0x9B]=L'ж';
    hashfromunicode[L'ж']=0x9B;
    hashtounicode[0x9C]=L'з';
    hashfromunicode[L'з']=0x9C;
    hashtounicode[0x9D]=L'и';
    hashfromunicode[L'и']=0x9D;
    hashtounicode[0x9E]=L'й';
    hashfromunicode[L'й']=0x9E;
    hashtounicode[0x9F]=L'Ÿ';
    hashfromunicode[L'Ÿ']=0x9F;
    hashtounicode[0xA1]=L'¡';
    hashfromunicode[L'¡']=0xA1;
    hashtounicode[0xA2]=L'к';
    hashfromunicode[L'к']=0xA2;
    hashtounicode[0xA3]=L'£';
    hashfromunicode[L'£']=0xA3;
    hashtounicode[0xA5]=L'л';
    hashfromunicode[L'л']=0xA5;
    hashtounicode[0xA6]=L'м';
    hashfromunicode[L'м']=0xA6;
    hashtounicode[0xA7]=L'н';
    hashfromunicode[L'н']=0xA7;
    hashtounicode[0xA8]=L'п';
    hashfromunicode[L'п']=0xA8;
    hashtounicode[0xA9]=L'т';
    hashfromunicode[L'т']=0xA9;
    hashtounicode[0xAA]=L'ф';
    hashfromunicode[L'ф']=0xAA;
    hashtounicode[0xAB]=L'ц';
    hashfromunicode[L'ц']=0xAB;
    hashtounicode[0xAC]=L'ч';
    hashfromunicode[L'ч']=0xAC;
    hashtounicode[0xAD]=L'ш';
    hashfromunicode[L'ш']=0xAD;
    hashtounicode[0xAE]=L'щ';
    hashfromunicode[L'щ']=0xAE;
    hashtounicode[0xAF]=L'ъ';
    hashfromunicode[L'ъ']=0xAF;
    hashtounicode[0xB0]=L'ы';
    hashfromunicode[L'ы']=0xB0;
    hashtounicode[0xB1]=L'ь';
    hashfromunicode[L'ь']=0xB1;
    hashtounicode[0xB2]=L'э';
    hashfromunicode[L'э']=0xB2;
    hashtounicode[0xB3]=L'ю';
    hashfromunicode[L'ю']=0xB3;
    hashtounicode[0xB4]=L'я';
    hashfromunicode[L'я']=0xB4;
    hashtounicode[0xB5]=L'Ő';
    hashfromunicode[L'Ő']=0xB5;
    hashtounicode[0xB6]=L'ő';
    hashfromunicode[L'ő']=0xB6;
    hashtounicode[0xB7]=L'Ű';
    hashfromunicode[L'Ű']=0xB7;
    hashtounicode[0xB8]=L'ű';
    hashfromunicode[L'ű']=0xB8;
}
WA_textcodec1252::~WA_textcodec1252(){}
QByteArray WA_textcodec1252::name () const{
    return "WormsArmageddon";
}
QList<QByteArray> WA_textcodec1252::aliases () const{
    return QList<QByteArray>()<<"wa";
}
int WA_textcodec1252::mibEnum () const{
    return 50001;
}
QString WA_textcodec1252::convertToUnicode ( const char * chars, int len, ConverterState *state) const{
    if(!chars)
        return QString();
    if(len==0)
        return QString();

    QTextCodec *codec=QTextCodec::codecForName("windows-1252");
    QString out;    
    for(int j=0; j<len; j++){        
        char i=chars[j];
        if(hashtounicode.contains(i)){
            out+=hashtounicode[i];
        }
        else
            out+=codec->toUnicode(&chars[j], 1, 0);
    }    
    return out;
}
QByteArray WA_textcodec1252::convertFromUnicode ( const QChar * input, int number, ConverterState *state) const{
    if(!input)
        return QByteArray();
    if(number==0)
        return QByteArray();
    QTextCodec *codec=QTextCodec::codecForName("windows-1252");
    QByteArray out;    
    for(int i=0;i<number;i++){
        int j = input[i].unicode();
        if(hashfromunicode.contains (j))
            out+=hashfromunicode[j];
        else
            out+= codec->fromUnicode (&input[i], 1 ,0);
    }
    return out;
}
WA_textcodec1251::~WA_textcodec1251(){}
QByteArray WA_textcodec1251::name () const{
    return "WormsArmageddon1251";
}
QList<QByteArray> WA_textcodec1251::aliases () const{
    return QList<QByteArray>()<<"wa2";
}
int WA_textcodec1251::mibEnum () const{
    return 50002;
}
QString WA_textcodec1251::convertToUnicode ( const char * chars, int len, ConverterState *state) const{
    if(!chars)
        return QString();
    if(len==0)
        return QString();

    QTextCodec *codec=QTextCodec::codecForLocale ();
    QString out;
    for(int j=0; j<len; j++){
        char i=chars[j];
        if(hashtounicode.contains(i)){
            out+=hashtounicode[i];
        }
        else
            out+=codec->toUnicode(&chars[j], 1, 0);
    }
    return out;
}
QByteArray WA_textcodec1251::convertFromUnicode ( const QChar * input, int number, ConverterState *state) const{
    if(!input)
        return QByteArray();
    if(number==0)
        return QByteArray();
    QTextCodec *codec=QTextCodec::codecForLocale ();
    QByteArray out;
    for(int i=0;i<number;i++){
        int j = input[i].unicode();
        if(hashfromunicode.contains (j))
            out+=hashfromunicode[j];
        else
            out+= codec->fromUnicode (&input[i], 1 ,0);
    }
    return out;
}
