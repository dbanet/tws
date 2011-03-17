#ifndef BUTTONLAYOUT_H
#define BUTTONLAYOUT_H

#include <QtGui/QWidget>
#include "ui_buttonlayout.h"

class buttonlayout : public QWidget
{
    Q_OBJECT

public:
    buttonlayout(QWidget *parent = 0);
    ~buttonlayout();
    void hidebuttons();
    void showbuttons();
    void fillleaguemenus();
private:
    Ui::buttonlayoutClass ui;
    QMenu *leaguemenu;
    QMenu *leaguestatemenu;
private slots:
    void on_pbsort_clicked();
    void on_pbballoon_clicked();
    void on_pbsound_clicked();
    void on_pbcostumwords_clicked();
    void leaguemenutriggered(QAction *action);
    void leaguestatemenutriggered(QAction*);
signals:
    void pbhostclicked();
    void pbminimizedclicked();
    void pbhideclicked();
    void sigchangealpha(int);
    void sigshowme();
    void sighideme();
    void sigchangeleaguestate();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // BUTTONLAYOUT_H
