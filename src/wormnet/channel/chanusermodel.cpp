#include "chanusermodel.h"
#include "../settings.h"
#include "../settingswindow.h"
#ifdef PHONON
#include "../sound_handler.h"
#endif
#include "../balloon_handler.h"
#include "../global_functions.h"
#include "../myDebug.h"
#include "../picturehandler.h"
#include "../leagueserverhandler.h"

#include <QTime>
#include <QApplication>
#include <QPointer>

#define COLUMNS 6

ChanUserModel::ChanUserModel(QString channel,QList<userstruct> *hosts,QObject *parent) :
    QAbstractItemModel(parent),
    _data(hosts),
    channel(channel){

    QString pictPath=QApplication::applicationDirPath()+"/snppictures/";
    if(!channelIcon   .load( pictPath + "channelicon.png"   )
     ||!userIcon      .load( pictPath + "usericon.png"      )
     ||!buddyIcon     .load( pictPath + "buddyicon.png"     )
     ||!ignoreIcon    .load( pictPath + "ignoreicon.png"    )
     ||!offlineIcon   .load( pictPath + "offlineicon.png"   )
     ||!awayUserIcon  .load( pictPath + "awayusericon.png"  )
     ||!awayBuddyIcon .load( pictPath + "awaybuddyicon.png" )
     ||!awayIgnoreIcon.load( pictPath + "awayignoreicon.png")
    ) myDebug()<<QObject::tr("Some Pictures are missing!");
}

int ChanUserModel::columnCount(const QModelIndex &/*parent*/) const{
    return COLUMNS;
}

int ChanUserModel::rowCount(const QModelIndex &/*parent*/) const{
    return _data->length();
}

Qt::ItemFlags ChanUserModel::flags(const QModelIndex &index) const{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QModelIndex ChanUserModel::parent(const QModelIndex &child) const{
    return QModelIndex();
}

QModelIndex ChanUserModel::index(int row,int column,const QModelIndex &parent) const{
    if(row<_data->length() && column<COLUMNS
    && row>=0              && column>=0)
        return this->createIndex(row,column);
    else
        return QModelIndex();
}

bool ChanUserModel::hasChildren(const QModelIndex &parent) const{
    if(parent.isValid())
        return false;
    else
        return true;
}

QVariant ChanUserModel::headerData(int section,Qt::Orientation orientation,int role) const{
    if(role==Qt::DisplayRole)
        if(orientation==Qt::Horizontal)
            switch(section){
                case 0: return QString(tr(""));
                case 1: return QString(tr(""));
                case 2: return QString(tr(""));
                case 3: return QString(tr("Nick"));
                case 4: return QString(tr("Clan"));
                case 5: return QString(tr("Client info"));
            }
    return QVariant();
}

QVariant ChanUserModel::data(const QModelIndex &index,int role) const{
    if  (!index.isValid()) return QVariant();
    int row   =index.row();
    int column=index.column();

    if (role==Qt::TextAlignmentRole)
        return int(Qt::AlignLeft|Qt::AlignVCenter);

    switch(column){
        case e_Flag:
            if(role==Qt::DecorationRole)
                return *singleton<pictureHandler>().getFlag(_data->at(row).country);
            else if(role==SortingRole)
                return _data->at(row).country;
            break;

        case e_Rank:
            if(role==Qt::DecorationRole)
                return getRank(_data->at(row));
            else if(role==Qt::ToolTipRole)
                return _data->at(row).rank;
            else if(role==SortingRole)
                return _data->at(row).rank;
            break;

        case e_Icon: // Nothing but just that decorative ignore/buddy/plain-user icon
            if(role==Qt::DecorationRole){
                if(S_S.buddylist.contains(_data->at(row).nick))
                    return buddyIcon;
                else if (S_S.ignorelist.contains(_data->at(row).nick))
                    return ignoreIcon;
                else
                    return userIcon;
            } else if(role==Qt::ToolTipRole){
                if(S_S.buddylist.contains(_data->at(row).nick))
                    return tr("Your buddy");
                else if (S_S.ignorelist.contains(_data->at(row).nick))
                    return tr("You ignore this badass");
                else
                    return tr("This user is neither in your Buddies list, nor in your Ignore list");
            } else if(role==SortingRole){
                if(S_S.buddylist.contains(_data->at(row).nick))
                    return "a"; // first go buddies
                else if (S_S.ignorelist.contains(_data->at(row).nick))
                    return "c"; // enemies ;) go last
                else
                    return "b"; // all others go in the middle ;)
            }
            break;

        case e_Nick:
            if(role==Qt::DisplayRole || role==SortingRole)
                return _data->at(row).nick;
            break;

        case e_Clan:
            if(role==Qt::DisplayRole || role==SortingRole)
                return getClan(_data->at(row));
            break;

        case e_Client: // User's client information
            if(role==Qt::DisplayRole || role==SortingRole)
                return _data->at(row).client;
            break;
    }

    return QVariant();
}

QMap<int,QVariant> ChanUserModel::itemData (const QModelIndex &index) const{
    QMap<int,QVariant> map=QAbstractItemModel::itemData(index);
    map[SortingRole]=data(index,SortingRole); /* adding our custom role SortingRole to */
    return map;       /* be able to sort both icon- (#0,1,2) and text-columns (#3,4,5) */
}

void ChanUserModel::usersChanged(void){
    emit layoutAboutToBeChanged();
    QModelIndex topLeft    =createIndex(0,0);
    QModelIndex bottomRight=createIndex(rowCount(),columnCount());
    emit layoutChanged();
    emit dataChanged(topLeft,bottomRight); // update the whole view :)
}

QVariant ChanUserModel::getClan(const userstruct &user) const{
    QString clan=S_S.spectateleagueserver?
                singleton<leagueserverhandler>().map_at_toString(user.nick,leagueserverhandler::e_clan):
                user.clan;
    if(clan.isEmpty()
     ||S_S.dissallowedclannames.contains(clan,Qt::CaseInsensitive))
        return QVariant();
    else return clan;
}

QVariant ChanUserModel::getRank(const userstruct &user) const{
    if(S_S.spectateleagueserver){
        QString rank=singleton<leagueserverhandler>().map_at_toString(user.nick,leagueserverhandler::e_rank);
        if(!rank.isEmpty())
            return *singleton<pictureHandler>().getLeagueRank(rank);
        if(S_S.cbonlyshowranksfromverifiedusers)
            return QVariant();
    }
    return *singleton<pictureHandler>().getRank(user.rank);
}

ChanUserModel::~ChanUserModel(){}
