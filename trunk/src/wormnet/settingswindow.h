#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include<QtGui/QWidget>

#include"global_macros.h"
#include"settings.h"
namespace Ui{
    class settingswindowClass;
}
class settingswindow : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(settingswindow);
public:
    void set(const QString&, const QVariant&);
private:
    QStringList objectnames;
    Ui::settingswindowClass *ui;  
private slots:
    void on_pbloaddefaultsounds_clicked();
    void on_pbloadsoundpack_clicked();
    void ok();
    void cancel();
    void soundoptionbuttonslot();
};

#endif // SETTINGSWINDOW_H
