/*
 * chathandler.h
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#ifndef CHATHANDLER_H_
#define CHATHANDLER_H_
#include<QObject>
#include<QPoint>
#include<QUrl>
#include<QTextCharFormat>
#include<QTextCursor>
#include<QMenu>
#include<QPointer>

class QTextBrowser;
class QTextDocument;
class usermessage;
class emoticonhandler;

class chathandler: public QObject {
	Q_OBJECT
public:
	chathandler(QObject*,QTextBrowser*,QString);
	virtual ~chathandler();        
        void append(const usermessage u);        
	void appenddebug(const QString&);	
	static void initialformatsaver();
        static void initialformatstarter();
        void insertText(const QString&,QTextCharFormat&,QString user=QString());

        void moveSliderToMaximum();

	bool slideratmaximum;     
        bool gotFirstMessage;
        static QHash<int, QTextCharFormat> hash;

        static QPointer<emoticonhandler> emot;

        bool eventFilter(QObject *obj, QEvent *event);
protected:
	QTextBrowser *tb;
	QTextDocument *doc;
        QTextCursor *cursor;
	QMenu fontmenu;
        QMenu chatmenu;
	QMenu wamenu;        

        const QString chatpartner;

        virtual QList<QPair<QVariant, QTextCharFormat> > getSegmentation(QString s, QTextCharFormat format);
        virtual QTextCharFormat getRightFormat(const usermessage u);
        static void initialformat(QTextCharFormat &format);
        QPair<QVariant, QTextCharFormat> makepair(QVariant v, QTextCharFormat format);

        static int whatsthispropertyId;
        static int userpropertyId;
        static int linkpropertyId;

        void get_new_font_and_color(QTextCharFormat *format);
        void get_new_font_and_color_with_chatwindow(QTextCharFormat *format);
        void get_new_font_and_color_with_walink(QTextCharFormat *format);
protected slots:
	virtual void contextrequest(const QPoint&);
	void anchorclicked(const QUrl&);
	void slidermoved(int);
//	void slidermovedbyaction(int);
	void usesettingswindow(const QString &s="");
	void setschememap(QTextCharFormat*,QFont font);
        void setschememap(QTextCharFormat*,QColor color);
        void setschememap(int i,QFont font);
        void setschememap(int i,QColor color);

        void selectionChanged();
signals:
	void sigopenchatwindow(const QString&);
private:
        void appendgarbage(usermessage u);
};

#endif /* CHATHANDLER_H_ */
