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
class QTextBrowser;
class QTextDocument;
class chathandler: public QObject {
	Q_OBJECT
public:
	chathandler(QObject*,QTextBrowser*,QString);
	virtual ~chathandler();
	void append(const QString&,const QString&,const QString&);
	void appendnotice(const QString&,const QString&,const QString&);
	void appenddebug(const QString&);
	void appendpartgarbage(const QString&);
	void appendjoingarbage(const QString&);
	void appendquitgarbage(const QString&);
	const QString channel;
	static void initialformatsaver();
	static void initialformatstarter();
        void insertText(const QString&,QTextCharFormat&,QString user=QString());

	bool slideratmaximum;
	static QTextCharFormat timeformat;
	static QTextCharFormat nickformat;
	static QTextCharFormat chatformat;
	static QTextCharFormat actionformat;
	static QTextCharFormat buddyformat;
	static QTextCharFormat noticeformat;
	static QTextCharFormat prvformat;
	static QTextCharFormat httpformat;
	static QTextCharFormat waformat;
	static QTextCharFormat debugformat;
	static QTextCharFormat garbagejoinformat;
	static QTextCharFormat garbagepartformat;
	static QTextCharFormat garbagequitformat;
	static QTextCharFormat garbageformat;
        static QTextCharFormat myselfformat;
protected:
	QTextBrowser *tb;
	QTextDocument *doc;
	QTextCursor *cursor;
	QMenu fontmenu;
	QMenu nickmenu;
	QMenu wamenu;
	QMenu prvmenu;
	QMenu noticemenu;
	QMenu debugmenu;

	static void initialformat(QTextCharFormat&,const QString&,const QString&);
        bool eventFilter(QObject *obj, QEvent *event);

protected slots:
	virtual void contextrequest(const QPoint&);
	void anchorclicked(const QUrl&);
	void slidermoved(int);
//	void slidermovedbyaction(int);
	void usesettingswindow(const QString &s="");
	void setschememap(QTextCharFormat*,QFont font);
	void setschememap(QTextCharFormat*,QColor color);
signals:
	void sigopenchatwindow(const QString&);
private:
        void get_new_font_and_color(QTextCharFormat *tempformat,QAction*);
};

#endif /* CHATHANDLER_H_ */
