#ifndef LOGBROWSER_H
#define LOGBROWSER_H

#include <QWidget>
#include "ui_logbrowser.h"

class LogBrowser : public QWidget
{
    Q_OBJECT

public:
    LogBrowser(QWidget *parent = 0);
    ~LogBrowser();

private:
    Ui::logbrowserClass ui;
private slots:
	void closeclicked();
	void chatclicked();
signals:
	void sigopenchatwindow(const QString&);
};

#endif // LOGBROWSER_H
