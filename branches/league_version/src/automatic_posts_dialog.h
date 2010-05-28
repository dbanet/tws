#ifndef AUTOMATIC_POSTS_DIALOG_H
#define AUTOMATIC_POSTS_DIALOG_H

#include <QDialog>

namespace Ui {
    class automatic_posts_dialog;
}

class automatic_posts_dialog : public QDialog {
    Q_OBJECT
public:
    automatic_posts_dialog(QWidget *parent = 0);
    ~automatic_posts_dialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::automatic_posts_dialog *ui;

private slots:
    void on_pbstop_clicked();
    void on_pbstart_clicked();
};

#endif // AUTOMATIC_POSTS_DIALOG_H
