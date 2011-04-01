#include"emoticonhandler.h"
#include"settingswindow.h"
#include"settings.h"
#include"myDebug.h"

#include<QMessageBox>
#include<QFileDialog>
#include<QFile>

emoticonhandler::emoticonhandler(QWidget *parent) :
	QWidget(parent) {
    ui.setupUi(this);
    scheme=S_S.getstring("smileyscheme");
    if(scheme.isEmpty()){
        scheme="default";
    }
    QDir dir(QApplication::applicationDirPath() + "/smileys");
    ui.cbscheme->addItems(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    QFile f;
    foreach(QString s,dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        f.setFileName(QApplication::applicationDirPath() + "/smileys/"+s+".smileyscheme");
        if (f.open(QFile::ReadOnly)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            ds >> filemap[s];
            f.close();
        }
    }
    ui.cbscheme->setCurrentIndex(ui.cbscheme->findText(scheme));
    foreach(QString s,filemap[scheme].keys()) {
        if (QFile::exists(QApplication::applicationDirPath() + "/smileys/" + scheme + "/" + filemap[scheme][s])) {
            map[s] = QImage(QApplication::applicationDirPath() + "/smileys/" + scheme + "/" + filemap[scheme][s]);
            QListWidgetItem *item = new QListWidgetItem(QIcon(QApplication::applicationDirPath() + "/smileys/"
                                                              + scheme + "/" + filemap[scheme][s]), s);
            ui.listWidget->addItem(item);
        }
    }
    ui.chbsmileys->setChecked(S_S.getbool("chbsmileys"));
    connect(ui.close, SIGNAL(clicked()),this, SLOT(myclose()));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(add()));
    connect(ui.remove, SIGNAL(clicked()),this, SLOT(remove()));
    connect(ui.cbscheme, SIGNAL(currentIndexChanged ( const QString &)),this, SLOT(indexofcbchanged(const QString&)));
}
//QVector<QVariant> emoticonhandler::preparemsg(QString msg) {
//    slhelper = map.keys();
//    return preparemsgrec(msg, 0);
//}
//QVector<QVariant> emoticonhandler::preparemsgrec(QString msg, int i) {
//    int h = -1;
//    QVector<QVariant> v;
//    if (i < slhelper.size()) {
//        h = msg.indexOf(slhelper[i]);
//        while (h != -1) {
//            v << preparemsgrec(msg.left(h), i + 1) << map[slhelper[i]];
//            msg = msg.remove(0, h + slhelper[i].size());
//            h = msg.indexOf(slhelper[i]);
//        }
//        if (!msg.isEmpty())
//            v << preparemsgrec(msg, i + 1);
//    } else
//        return QVector<QVariant> () << msg;
//    return v;
//}
void emoticonhandler::myclose() {
    scheme = ui.cbscheme->currentText();
    foreach(QString s,filemap.keys()) {
        QFile f;
        f.setFileName(QApplication::applicationDirPath() + "/smileys/" + s + ".smileyscheme");
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            ds << filemap[s];
        }
    }
    singleton<settingswindow>().set("showsmileysinchannels",!ui.chbsmileys->isChecked());
    singleton<settingswindow>().set("chbsmileysinchatwindows",!ui.chbsmileys->isChecked());
    close();
}
void emoticonhandler::add() {
    if (ui.lineEdit->text().simplified() != "") {
        if (ui.listWidget->findItems(ui.lineEdit->text().simplified(), Qt::MatchCaseSensitive).size() == 0) {
            QString s = QFileDialog::getOpenFileName(0, tr(
                    "Choose a Smiley icon"), QApplication::applicationDirPath()
                                                     + "/smileys/" + scheme + "/", "*.gif; *.png");
            map[ui.lineEdit->text().simplified()] = QImage(s);
            filemap[scheme][ui.lineEdit->text().simplified()] = QFileInfo(s).fileName();
            QListWidgetItem *item = new QListWidgetItem(QIcon(s), ui.lineEdit->text().simplified());
            ui.listWidget->addItem(item);
        } else
            QMessageBox::warning(0, QObject::tr("Warning"), tr(
                    "This shortcut is allready reserved."));
    } else {
        QMessageBox::warning(
                0,
                QObject::tr("Warning"),
                tr("Bevor you can choose a smiley, you must declare a smiley shortcut."));
    }
}
void emoticonhandler::remove() {
    QListWidgetItem *item;
    if (!ui.listWidget->selectedItems().isEmpty()) {
        item = ui.listWidget->selectedItems().first();
        myDebug() << item->text();
        filemap[scheme].remove(item->text());
        map.remove(item->text());
        ui.listWidget->clear();
        foreach(QString s,filemap[scheme].keys()) {
            if (QFile::exists(QApplication::applicationDirPath()
                + "/smileys/" + scheme + "/" + filemap[scheme][s])) {
                map[s] = QImage(QApplication::applicationDirPath()
                                + "/smileys/" + scheme + "/" + filemap[scheme][s]);
                QListWidgetItem *item = new QListWidgetItem(QIcon(
                        QApplication::applicationDirPath() + "/smileys/"
                        + scheme + "/" + filemap[scheme][s]), s);
                ui.listWidget->addItem(item);
            }

        }
    }
}
void emoticonhandler::indexofcbchanged(const QString&) {
    map.clear();
    ui.listWidget->clear();
    scheme = ui.cbscheme->currentText();
    S_S.set("smileyscheme", scheme);
    foreach(QString s,filemap[scheme].keys()) {
        if (QFile::exists(QApplication::applicationDirPath() + "/smileys/"
                          + scheme + "/" + filemap[scheme][s])) {
            map[s] = QImage(QApplication::applicationDirPath()
                            + "/smileys/" + scheme + "/" + filemap[scheme][s]);
            QListWidgetItem *item = new QListWidgetItem(QIcon(
                    QApplication::applicationDirPath() + "/smileys/"
                    + scheme + "/" + filemap[scheme][s]), s);
            ui.listWidget->addItem(item);
        }

    }
}
QVariant emoticonhandler::contains(QString s){
    if(map.contains(s))
        return map[s];
    return s;
}
emoticonhandler::~emoticonhandler() {

}
