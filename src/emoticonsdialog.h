#ifndef EMOTICONSDIALOG_H
#define EMOTICONSDIALOG_H

#include <QWidget>
class QListWidgetItem;
namespace Ui {
    class emoticonsdialog;
}

class emoticonsdialog : public QWidget
{
    Q_OBJECT

public:
    explicit emoticonsdialog(QWidget *parent = 0);
    ~emoticonsdialog();

private:
    Ui::emoticonsdialog *ui;
protected:
    void focusOutEvent ( QFocusEvent * event );
private slots:
    void smileyclicked(QListWidgetItem *);
signals:
    void sigemotchoosed(QString);
};

#endif // EMOTICONSDIALOG_H
