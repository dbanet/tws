#ifndef CODECSELECTDIA_H
#define CODECSELECTDIA_H

#include <QDialog>

namespace Ui {
    class CodecSelectDia;
}

class CodecSelectDia : public QDialog
{
    Q_OBJECT

public:
    explicit CodecSelectDia(QWidget *parent = 0);
    ~CodecSelectDia();
    static QTextCodec *codec;
    static bool contains(QString s);
    static void setcodec(QString s);   

private:
    Ui::CodecSelectDia *ui;

private slots:
    void on_pbOk_clicked();
    void on_pbcyrillic_clicked();
    void on_pblatin_clicked();
};

#endif // CODECSELECTDIA_H
