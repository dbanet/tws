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
private:
    Ui::buttonlayoutClass ui;
private slots:
    void on_pbsort_clicked();
    void on_pbballoon_clicked();
    void on_pbsound_clicked();
    void on_pbcostumwords_clicked();
signals:
    void pbhostclicked();
    void pbminimizedclicked();
    void pbhideclicked();
    void sigchangealpha(int);
    void sigshowme();
    void sighideme();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // BUTTONLAYOUT_H
