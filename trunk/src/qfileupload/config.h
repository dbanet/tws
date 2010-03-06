#ifndef CONFIG_H_
#define CONFIG_H_
#include "ui_config.h"
#include <QtGui>

class ConfigDialog : public QWidget, public Ui::config
{
Q_OBJECT
public:
	ConfigDialog(QWidget* parent = 0); //Constructor
	void init(QColor l, QColor e, QColor o, QColor v1, QColor v2);
	QColor link, verlauf1, verlauf2, log, err;
public slots:
	void changeColor()
	{
		QPushButton *button = qobject_cast<QPushButton*>(sender());
		QColor color = QColorDialog::getColor(Qt::black, this);
		QPixmap pxm(32,32);
		pxm.fill(color);
		button->setIcon(QIcon(pxm));
		if(button == linkButton)
			link = color;
		if(button == errButton)
			err = color;
		if(button == logButton)
			log = color;
		if(button == verlauf1Button)
			verlauf1 = color;
		if(button == verlauf2Button)
			verlauf2 = color;
		emit save();
	}
signals:
	void save();
};
#endif
