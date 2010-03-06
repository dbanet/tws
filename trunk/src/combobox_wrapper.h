#ifndef COMBOBOX_WRAPPER_H
#define COMBOBOX_WRAPPER_H

#include <QWidget>

namespace Ui {
    class combobox_wrapper;
}

class combobox_wrapper : public QWidget {
    Q_OBJECT
public:
    combobox_wrapper(QWidget *parent = 0);
    ~combobox_wrapper();

    Ui::combobox_wrapper *ui;
    void set(QStringList);
    QStringList get();

private slots:
    void on_down_clicked();
    void on_up_clicked();
    void on_minus_clicked();
    void on_plus_2_clicked();
};

#endif // COMBOBOX_WRAPPER_H
