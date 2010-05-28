#ifndef UPLOADEDFILEATOMIC_H
#define UPLOADEDFILEATOMIC_H

#include <QtGui/QWidget>
#include "ui_uploadedfileatomic.h"

class uploadedfileatomic : public QWidget
{
    Q_OBJECT

public:
    uploadedfileatomic(const QString&,const QString&,QWidget *parent = 0);
    ~uploadedfileatomic();
private slots:
	void copyclicked();
	void delclicked();
private:
	QString link;
    Ui::uploadedfileatomicClass ui;
};

#endif // UPLOADEDFILEATOMIC_H
