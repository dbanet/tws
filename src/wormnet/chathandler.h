/*
 * chathandler.h
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#ifndef CHATHANDLER_H_
#define CHATHANDLER_H_
#include <QObject>
#include <QPoint>
#include <QUrl>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QMenu>
#include <QPointer>

class QTextBrowser;
class QTextDocument;
class usermessage;
class emoticonhandler;

class chatHandler: public QObject {
	Q_OBJECT
public:
	chatHandler(QObject*,QTextBrowser*,QString);
	virtual ~chatHandler();        
        void append(const usermessage u);        
	void appenddebug(const QString&);	
	static void initialformatsaver();
        static void initialFormatStarter();
        void insertText(const QString&,QTextCharFormat&,QString user=QString());

        void moveSliderToMaximum();

	bool sliderAtMaximum;     
        bool gotFirstMessage;
        static QHash<int, QTextCharFormat> hash;

        static QPointer<emoticonhandler> emot;

        bool eventFilter(QObject *obj, QEvent *event);
protected:
	QTextBrowser *tb;
	QTextDocument *doc;
        QTextCursor *cursor;
	QMenu fontMenu;
        QMenu chatMenu;
	QMenu wamenu;        

        const QString chatpartner;

        virtual QList<QPair<QVariant, QTextCharFormat> > getSegmentation(QString s, QTextCharFormat format);
        virtual QTextCharFormat getRightFormat(const usermessage u);
        static void initialFormat(QTextCharFormat &format);
        QPair<QVariant, QTextCharFormat> makePair(QVariant v, QTextCharFormat format);

        static int whatsthispropertyId;
        static int userpropertyId;
        static int linkpropertyId;

        void get_new_font_and_color(QTextCharFormat *format);
        void getNewFontAndColorWithChatwindow(QTextCharFormat *format);
        void getNewFontAndColorWithWalink(QTextCharFormat *format);

        bool isprv;
protected slots:
	virtual void contextrequest(const QPoint&);
	void anchorClicked(const QUrl&);
	void slidermoved(int);
//	void slidermovedbyaction(int);
	void useSettingsWindow(const QString &s="");
	void setSchemeMap(QTextCharFormat*,QFont font);
        void setSchemeMap(QTextCharFormat*,QColor color);
        void setSchemeMap(int i,QFont font);
        void setSchemeMap(int i,QColor color);

        void selectionChanged();
signals:
	void sigOpenChatWindow(const QString&);
private:
        void appendGarbage(usermessage u);
};

#endif /* CHATHANDLER_H_ */
