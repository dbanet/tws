#ifndef INITEXTBROWSER_H
#define INITEXTBROWSER_H

#include <QtGui/QWidget>
#include "ui_initextbrowser.h"
class QHBoxLayout;
class initextbrowser : public QWidget
{
    Q_OBJECT

public:
    initextbrowser(QWidget *parent = 0);
    ~initextbrowser();
    void start(const QString &);
private:
	QString inifilename;
	QHBoxLayout *l;
    Ui::initextbrowserClass ui;
private slots:
	void save();
};

#endif // INITEXTBROWSER_H
