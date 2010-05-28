#include "buddylist.h"
#include"snpsettings.h"

buddylist::buddylist(QWidget *parent) :
	QWidget(parent) {
	this->setObjectName("normalwidget");
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	connect(ui.close, SIGNAL(clicked()),this, SLOT(close()));
	connect(ui.add, SIGNAL(clicked()),this, SLOT(add()));
	connect(ui.remove, SIGNAL(clicked()),this, SLOT(remove()));
        ui.list->addItems(singleton<snpsettings>().map["buddylist"].value<QStringList> ());
}
void buddylist::add() {
        QStringList sl = singleton<snpsettings>().map["buddylist"].value<QStringList> ();
	if (!sl.contains(ui.le->text(), Qt::CaseInsensitive)
			&& !ui.le->text().isEmpty()) {
		sl << ui.le->text();
                singleton<snpsettings>().map["buddylist"].setValue<QStringList> (sl);
		ui.list->clear();
                ui.list->addItems(singleton<snpsettings>().map["buddylist"].value<QStringList> ());
                singleton<snpsettings>().safe();
	}
}
void buddylist::remove() {
	QList<QListWidgetItem*> il = ui.list->selectedItems();
	if (il.size() > 0) {
		QString s = il.first()->text();
                QStringList sl = singleton<snpsettings>().map["buddylist"].value<QStringList> ();
		sl.removeAll(s);
                singleton<snpsettings>().map["buddylist"].setValue<QStringList> (sl);
		ui.list->clear();
                ui.list->addItems(singleton<snpsettings>().map["buddylist"].value<QStringList> ());
                singleton<snpsettings>().safe();
	}
}
buddylist::~buddylist() {

}
