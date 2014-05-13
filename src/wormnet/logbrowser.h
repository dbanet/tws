#ifndef LOGBROWSER_H
#define LOGBROWSER_H

#include <QWidget>
#include "ui_logbrowser.h"

class logbrowser : public QWidget
{
    Q_OBJECT

public:
    logbrowser(QWidget *parent = 0);
    ~logbrowser();

private:
    Ui::logbrowserClass ui;
private slots:
	void closeclicked();
	void chatclicked();
signals:
	void sigOpenChatWindow(const QString&);
};

#endif // LOGBROWSER_H
