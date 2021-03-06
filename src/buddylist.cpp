#include "buddylist.h"
#include "settings.h"
#include "global_functions.h"

BuddyList::BuddyList(QWidget *parent) :
	QWidget(parent) {
    setObjectName("normalwidget");
    setAttribute(Qt::WA_DeleteOnClose);
    ui.setupUi(this);
    connect(ui.close, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(add()));
    connect(ui.remove, SIGNAL(clicked()),this, SLOT(remove()));
    ui.list->addItems(S_S.buddylist);
}
void BuddyList::add() {
    QStringList sl = S_S.buddylist;
    if (!containsCI(sl, ui.le->text()) && !ui.le->text().isEmpty()) {
        sl << ui.le->text();
        S_S.set("buddylist", sl);
        ui.list->clear();
        ui.list->addItems(S_S.buddylist);
    }
}
void BuddyList::remove() {
    QList<QListWidgetItem*> il = ui.list->selectedItems();
    if (il.size() > 0) {
        QString s = il.first()->text();
        QStringList sl = S_S.buddylist;
        sl.removeAll(s);
        S_S.set("buddylist", sl);
        ui.list->clear();
        ui.list->addItems(S_S.buddylist);
        ui.list->setCurrentRow(0);
    }
}
