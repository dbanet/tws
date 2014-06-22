#ifndef IRCJOINDIA_H
#define IRCJOINDIA_H

#include <QDialog>

namespace Ui {
    class ircJoinDia;
}

class ircJoinDia : public QDialog
{
    Q_OBJECT

public:
    explicit ircJoinDia(QWidget *parent = 0);
    ~ircJoinDia();

    QString getChannel();
private:
    Ui::ircJoinDia *ui;
};

#endif // IRCJOINDIA_H
