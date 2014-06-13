#ifndef HOSTMODEL_H
#define HOSTMODEL_H
#include <QList>
#include <QModelIndex>
#include <QPixmap>

#include "../hoststruct.h"

class HostModel : public QAbstractItemModel{
    Q_OBJECT

public:
    HostModel                   (QString channel,QList<hoststruct> *hosts,QObject *parent=0);

    enum{                          /* this role is used to assign small pieces of text to  */
        SortingRole=Qt::UserRole+1 /* items for the sorter proxy model to be able to sort  */
    };                             /* items with no DisplayRole (e. g. flags and ranks)    */

    int            rowCount      (const QModelIndex &parent=QModelIndex())             const;
    int            columnCount   (const QModelIndex &parent=QModelIndex())             const;
    Qt::ItemFlags  flags         (const QModelIndex &index)                            const;
    QModelIndex    parent        (const QModelIndex &child)                            const;
    QModelIndex    index         (int row, int column, const QModelIndex &parent)      const;
    QMap<int
        ,QVariant> itemData      (const QModelIndex &index )                           const;
    QVariant       headerData    (int section,Qt::Orientation orientation,int role)    const;
    QVariant       data          (const QModelIndex &index,int role=Qt::DisplayRole)   const;

    void           hostsChanged  (void)                                                     ;
    ~HostModel();

private:
    /* _data is an internal private pointer to a QList of hoststructs */
    /* A hoststruct fully describes a game host, including gamename,  */
    /* hoster name, hoster IP address, flag, status (open or private) */
    /* and all other needed information. The model instantiates with  */
    /* such a pointer, and a channel name. The constructor saves the  */
    /* given information to the following private fields. The Model   */
    /* must be a private member of a channelTab, but the QList of     */
    /* hoststructs the _data pointer references must be a *public*    */
    /* member of channelTab, as is the channel name QString. When     */
    /* the QList of hoststruct the _data pointer references gets      */
    /* changed (for example, a new host gets hosted), the public      */
    /* function void HostModel::hostsChanged(void) must be called to  */
    /* inform the HostModel that the core data structure has been     */
    /* modified. The Model then will emit the dataChanged signal, so  */
    /* that the correspoinding View updates the visible information.  */
    /* The whole view gets updated in this case (emit dataChagned(    */
    /* createIndex(0,0),createIndex(rowCount(),columnCount());).      */
    QList<hoststruct> *_data;

    /* The Model displays those hoststructs only, whose chan() public */
    /* method returns a QString that equals to the following private  */
    /* field (which is set during instantiation).                     */
    QString channel;

    /* Miscellaneous thingies: icons                                  */
    QPixmap channelIcon;
    QPixmap hostIcon;
    QPixmap buddyHostIcon;
    QPixmap ignoreHostIcon;
};

#endif // HOSTMODEL_H
