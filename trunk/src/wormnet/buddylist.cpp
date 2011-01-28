#include"buddylist.h"
#include"snpsettings.h"
#include"global_functions.h"

buddylist::buddylist(QWidget *parent) :
	QWidget(parent) {
	this->setObjectName("normalwidget");
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	connect(ui.close, SIGNAL(clicked()),this, SLOT(close()));
	connect(ui.add, SIGNAL(clicked()),this, SLOT(add()));
	connect(ui.remove, SIGNAL(clicked()),this, SLOT(remove()));
        ui.list->addItems(S_S.getstringlist("buddylist"));
}
void buddylist::add() {
        QStringList sl = S_S.getstringlist("buddylist");
        if (!containsCI(sl, ui.le->text()) && !ui.le->text().isEmpty()) {
		sl << ui.le->text();
                S_S.set("buddylist", sl);
		ui.list->clear();
                ui.list->addItems(S_S.getstringlist("buddylist"));
                S_S.safe();
	}
}
void buddylist::remove() {
	QList<QListWidgetItem*> il = ui.list->selectedItems();
	if (il.size() > 0) {
		QString s = il.first()->text();
                QStringList sl = S_S.getstringlist("buddylist");
		sl.removeAll(s);
                S_S.set("buddylist", sl);
		ui.list->clear();
                ui.list->addItems(S_S.getstringlist("buddylist"));
                S_S.safe();
	}
}
