#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include<QtGui/QWidget>
#include<QMap>
#include"global_macros.h"
namespace Ui{
    class settingswindowClass;
}
class settingswindow : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(settingswindow);
public:
    QStringList objectnames;
    void safe();
    void load();
    const QVariant from_map(const QString&) const;
    void to_map(const QString&, const QVariant&);
private:
    Ui::settingswindowClass *ui;
    QMap<QString,QVariant> map;
    void loadDefaults();
    void checkValidEntries();
private slots:
    void on_pbloaddefaultsounds_clicked();
    void on_pbloadsoundpack_clicked();
    void ok();
    void cancel();
    void soundoptionbuttonslot();
};

#endif // SETTINGSWINDOW_H
