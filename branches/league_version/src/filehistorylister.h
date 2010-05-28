#ifndef FILEHISTORYLISTER_H
#define FILEHISTORYLISTER_H

#include <QtGui/QFrame>
#include "ui_filehistorylister.h"

class filehistorylister : public QFrame
{
    Q_OBJECT

public:
    filehistorylister(QFrame *parent = 0);
    ~filehistorylister();
private slots:
	void closeclicked();
private:
    Ui::filehistorylisterClass ui;
};

#endif // FILEHISTORYLISTER_H
