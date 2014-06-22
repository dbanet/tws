#ifndef irc_usermodel_H
#define irc_usermodel_H
#include <QAbstractItemModel>
#include <QPixmap>
#include "src/wormnet/userstruct.h"
class irc_netcoupler;
class irc_usermodel: public QAbstractItemModel {
    Q_OBJECT
public:
    enum {
        e_Nick=0,e_Flag,e_Rank,e_Clan,e_Client, e_Channel=999
    };
    irc_usermodel(irc_netcoupler*);
    ~irc_usermodel();

    void setuserstruct(QMap<QString,QList<userstruct> >);
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role =
                        Qt::DisplayRole) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex & index ) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

    void addbuddy(const QString&);
    void deletebuddy(const QString&);
    void addignore(const QString&);
    void deleteignore(const QString&);

    void selectionchanged(const QModelIndex &,const QWidget*);

    userstruct getuserstructbyindex(const QModelIndex&);

    QModelIndex indexbychannelname(QString);
    QStringList classes;
    QMap<QString,QList<userstruct> > usermap;

private:
    QString currentselecteduser;
    int currentselectedchannel;
    QMap<QString,const QWidget*> selectionwidgetmap;

    QStringList stringnamelist;
    bool *boolean;

    Qt::SortOrder sortorder;
    int sortsection;

    QPixmap channelicon;
    QPixmap usericon;
    QPixmap buddyicon;
    QPixmap ignoreicon;
    QPixmap offlineicon;
    QPixmap awayusericon;
    QPixmap awaybuddyicon;
    QPixmap awayignoreicon;

    QStringList currentbuddylist;
    unsigned int whichorder;
    irc_netcoupler *irc_net;
private slots:
    void buddyarrived();
    void buddyleft();
public slots:
    void sortslot(int, Qt::SortOrder);
    void usesettingswindow(const QString &s="");
signals:
    void sigselectitem(const QModelIndex&,const QWidget*);
    void sigbuddyleft();
    void sigbuddyarrived();
};
#endif // irc_usermodel_H
