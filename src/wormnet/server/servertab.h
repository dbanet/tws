#ifndef SERVERTAB_H
#define SERVERTAB_H

#include <QMainWindow>
#include <QPointer>

#include "../mainwindow.h"
class MainWindow;

namespace Ui {
    class serverTab;
}

class serverTab : public QMainWindow
{
    Q_OBJECT

public:
    explicit serverTab(MainWindow *mainWnd,QWidget *parent = 0);
    void fillSnpSettings();
    void returnToLoginTab();
    void addToServInfo(QString);
    void set_chbautojoin(bool);
    void set_chbminimized(bool);
    ~serverTab();

private slots:
    void on_pbjoin_clicked();

    void on_pbrememberjoin_clicked();

    void on_chbautojoin_clicked(bool checked);

private:
    Ui::serverTab *ui;
    QPointer<MainWindow> mainWnd;
    void gotChannelList(const QStringList&);
    QStringList channelList;
};

#endif // SERVERTAB_H
