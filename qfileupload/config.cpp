#include "config.h"
/*!
Konstruktor.
Buttons mit Slots verbinden und alle Variablen initialisieren.
Mit der Standart-Datenbank verbinden
*/
ConfigDialog::ConfigDialog(QWidget* parent /* = 0 */) //Konstruktor
: QWidget(parent)
{

	setupUi(this);
	connect(linkButton, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(errButton, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(logButton, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(verlauf1Button, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(verlauf2Button, SIGNAL(clicked()), this, SLOT(changeColor()));

}

void ConfigDialog::init(QColor l, QColor e, QColor o, QColor v1, QColor v2)
{
	
		QPixmap pxm(32,32);
		pxm.fill(l);
		linkButton->setIcon(QIcon(pxm));
		link = l;
		
		pxm.fill(e);
		errButton->setIcon(QIcon(pxm));
		err = e;
		
		pxm.fill(o);
		logButton->setIcon(QIcon(pxm));
		log = o;
		
		pxm.fill(v1);
		verlauf1Button->setIcon(QIcon(pxm));
		verlauf1 = v1;
		
		pxm.fill(v2);
		verlauf2Button->setIcon(QIcon(pxm));
		verlauf2 = v2;
} 
