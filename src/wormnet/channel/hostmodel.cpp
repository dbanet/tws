#include "hostmodel.h"
#include "../settings.h"
#include "../settingswindow.h"
#ifdef PHONON
#include "../sound_handler.h"
#endif
#include "../balloon_handler.h"
#include "../global_functions.h"
#include "../myDebug.h"
#include "../picturehandler.h"

#include <QTime>
#include <QApplication>

#define COLUMNS 5

HostModel::HostModel(QString channel,QList<hoststruct> *hosts,QObject *parent) :
    QAbstractItemModel(parent),
    _data(hosts),
    channel(channel){

    QString pictPath=QApplication::applicationDirPath()+"/snppictures/";
    if(!channelIcon   .load( pictPath + "channelicon.png"   )
     ||!hostIcon      .load( pictPath + "hosticon.png"      )
     ||!buddyHostIcon .load( pictPath + "buddyhosticon.png" )
     ||!ignoreHostIcon.load( pictPath + "ignorehosticon.png")
    ) myDebug()<<QObject::tr("Some Pictures are missing!");
}

int HostModel::columnCount(const QModelIndex &/*parent*/) const{
    return COLUMNS;
}

int HostModel::rowCount(const QModelIndex &/*parent*/) const{
    return _data->length();
}

QVariant HostModel::headerData(int section,Qt::Orientation orientation,int role) const{
    if(role==Qt::DisplayRole)
        if(orientation==Qt::Horizontal)
            switch(section){
                case 0: return QString(tr("Flag"));
                case 1: return QString(tr("Status"));
                case 2: return QString(tr("Gamename"));
                case 3: return QString(tr("Hoster"));
                case 4: return QString(tr("IP"));
            }
    return QVariant();
}

QVariant HostModel::data(const QModelIndex &index,int role) const{
    if (!index.isValid()) return QVariant();
    int row   =index.row();
    int column=index.column();

    if (role==Qt::TextAlignmentRole)
        return int(Qt::AlignLeft|Qt::AlignVCenter);

    if (role==Qt::DecorationRole)
        if(column==0)
            if (S_S.buddylist.contains(_data->at(row).nick()))
                return buddyHostIcon;
            else if (S_S.ignorelist.contains(_data->at(row).nick()))
                return ignoreHostIcon;
            else
                return hostIcon;

    if (role==Qt::DisplayRole || role==Qt::BackgroundRole)
        switch(column){
            case 0: // Gamename
                if(role==Qt::DisplayRole)
                    return _data->at(row).name();

            case 1: // Hoster's nick
                if(role==Qt::DisplayRole)
                    return _data->at(row).nick();

            case 2: // Hoster's flag
                if(role==Qt::BackgroundRole)
                    return *singleton<pictureHandler>().getFlag(_data->at(row).country());

            case 3: // Game's state (open or private)
                if(role==Qt::BackgroundRole)
                    return _data->at(row).withkey()?
                        *singleton<pictureHandler>().locked:
                        *singleton<pictureHandler>().unlocked;

            case 4: // Hoster's IP address
                if(role==Qt::DisplayRole)
                    return _data->at(row).ip();
        }

    return QVariant();
}
