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
    QAbstractListModel(parent),
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
    switch(index.column()){
        case 0:
        case 1:
            return Qt::ItemIsEnabled;

        default:
            return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    }
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

    if (role==Qt::DecorationRole)
        if(column==2){
            if (S_S.buddylist.contains(_data->at(row).nick))
                return buddyIcon;
            else if (S_S.ignorelist.contains(_data->at(row).nick))
                return ignoreIcon;
            else
                return userIcon;
        }

    if (role==Qt::DisplayRole || role==Qt::BackgroundRole)
        switch(column){
            case e_Flag:
                if(role==Qt::BackgroundRole)
                    return *singleton<pictureHandler>().getFlag(_data->at(row).country);
                break;

            case e_Rank:
                if(role==Qt::BackgroundRole)
                    return getRank(_data->at(row));
                break;

            case e_Icon: // Nothing but just that decorative ignore/buddy/plain-user icon
                break;

            case e_Nick:
                if(role==Qt::DisplayRole)
                    return _data->at(row).nick;
                break;

            case e_Clan:
                if(role==Qt::DisplayRole)
                    return getClan(_data->at(row));
                break;

            case e_Client: // User's client information
                if(role==Qt::DisplayRole)
                    return _data->at(row).client;
                break;
        }

    return QVariant();
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
