#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
    class progressdialog;
}

class progressdialog : public QDialog
{
    Q_OBJECT

public:
    explicit progressdialog(QWidget *parent = 0);
    ~progressdialog();

private:
    Ui::progressdialog *ui;
};

#endif // PROGRESSDIALOG_H
