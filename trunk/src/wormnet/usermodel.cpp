#include "usermodel.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include<QtGui>
#include"netcoupler.h"
#include"ctcphandler.h"
#include "sound_handler.h"
#include"global_functions.h"
#include"clantowebpagemapper.h"
#include"leagueserverhandler.h"
extern QPointer<netcoupler> net;
extern QList<QPixmap*> flaglist; //declared in main.cpp
extern QList<QPixmap*> ranklist; //declared in main.cpp
extern QList<QPixmap*> leagueranklist;
extern QStringList querylist;
extern QString GamesourgeChannelName;
QStringList usermodel::buddyarrivedhelper;
QStringList usermodel::buddylefthelper;

usermodel::usermodel(QObject * parent) :
	QAbstractItemModel(parent) {
    stringnamelist << tr("Nick") << "" << tr("Rank") << tr("Clan") << tr("Information");

    sortorder = Qt::AscendingOrder;
    sortsection = 0;

    if (!channelicon.load(QApplication::applicationDirPath()
        + "/snppictures/channelicon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!usericon.load(QApplication::applicationDirPath()
        + "/snppictures/usericon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!buddyicon.load(QApplication::applicationDirPath()
        + "/snppictures/buddyicon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!ignoreicon.load(QApplication::applicationDirPath()
        + "/snppictures/ignoreicon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!offlineicon.load(QApplication::applicationDirPath()
        + "/snppictures/offlineicon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!awayusericon.load(QApplication::applicationDirPath()
        + "/snppictures/awayusericon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    if (!awaybuddyicon.load(QApplication::applicationDirPath()
        + "/snppictures/awaybuddyicon.png"))
        myDebug() << tr("Some Pictures are missing!");
    if (!awayignoreicon.load(QApplication::applicationDirPath()
        + "/snppictures/awayignoreicon.png"))
        myDebug() << QObject::tr("Some Pictures are missing!");
    currentselectedchannel = -1;
    usesettingswindow();
    //connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow())); ********in netcoupler
    connect(this, SIGNAL(sigbuddyleft()),this, SLOT(buddyleft()));
    connect(this, SIGNAL(sigbuddyarrived()),this, SLOT(buddyarrived()));
}
void usermodel::selectionchanged(const QModelIndex &index, const QWidget *w) {

    selectionwidgetmap.remove(currentselecteduser);
    currentselecteduser = data(index.sibling(index.row(), 0)).value<QString> ();
    this->currentselectedchannel = index.internalId();
    selectionwidgetmap[currentselecteduser] = w;
}
void usermodel::setuserstruct(const QList<userstruct> &upar, QMap<QString,
                              QStringList> joinlist) {

    emit layoutAboutToBeChanged();
    users = upar;
    usermap.clear();
    foreach(QString s,usermap_channellist_helper){
        usermap[s];
    }
    foreach(userstruct u,users) {
        joinlist[u.chan].removeAll(u.nick);
        usermap[u.chan].push_back(u);
    }
    int i;
    foreach(QString channel,joinlist.keys()) {
        foreach(QString s,joinlist[channel]) {
            i = users.indexOf(userstruct::whoami(s));
            if (i > -1)
                usermap[channel].push_back(users[i]);
        }
    }
    QStringList sl;
    foreach(QString s,singleton<snpsettings>().map["buddylist"].value<QStringList>()) {
        if (users.contains(userstruct(userstruct::whoami(s)))) {
            usermap[tr("Buddylist")].push_back(users[users.indexOf(
                    userstruct::whoami(s))]);
            sl << s;
            if (currentbuddylist.removeAll(s) == 0) { //a buddy arrived
                buddyarrivedhelper<< QTime::currentTime().toString("hh:mm") + ":" + s+tr(" connected to wormnet.");
                emit sigbuddyarrived();
            }
        }
    }
    if (currentbuddylist.size() != 0){ //buddy left
        foreach(QString s,currentbuddylist){
            buddylefthelper<< QTime::currentTime().toString("hh:mm") + ":" + s+tr(" left wormnet.");
        }
        emit sigbuddyleft();
        foreach(QString s,currentbuddylist){
            removeCI(ctcphandler::awayusers,s);
        }
    }
    currentbuddylist = sl;
    foreach(QString s,querylist) {
        if ( users.contains(userstruct(userstruct::whoami(s))) )
            usermap[tr("Querys")].push_back(users[users.indexOf(
                    userstruct::whoami(s))]);
        else
            usermap[tr("Querys")].push_back(userstruct(QStringList() << ""
                                                       << "" << "" << "" << s));
    }
    usermap[tr("Buddylist")];
    usermap[tr("Querys")];
#ifdef WITH_GAMESURGE_SUPPORT
    usermap[GamesourgeChannelName];
#endif
    usermap[usermodel::tr("Ignorelist")];
    foreach(QString s,singleton<snpsettings>().map["ignorelist"].value<QStringList>()) {
        usermap[usermodel::tr("Ignorelist")].push_back(userstruct(
                QStringList() << "" << "" << "" << "" << s));
    }
    classes = usermap.keys();
    classes.move(classes.indexOf(tr("Buddylist")), 0);
    classes.move(classes.indexOf(tr("Querys")), 1);
#ifdef WITH_GAMESURGE_SUPPORT
    classes.move(classes.indexOf(GamesourgeChannelName), classes.length()-1);
#endif
    sort(sortsection, sortorder);
    if (currentselectedchannel > -1 && currentselectedchannel < classes.size()) {
        int row = usermap[classes[currentselectedchannel]].indexOf(
                userstruct::whoami(currentselecteduser));
        if (row == -1) {
            currentselectedchannel = -1;
            emit sigselectitem(QModelIndex(),
                               selectionwidgetmap[currentselecteduser]);
        } else {
            QModelIndex in = createIndex(row, 0, currentselectedchannel);
            emit sigselectitem(in, selectionwidgetmap[currentselecteduser]);
        }
    }
}
void usermodel::addbuddy(const QString &user) {

    emit
            layoutAboutToBeChanged();
    if (!singleton<snpsettings>().map["buddylist"].value<QStringList> ().contains(user,
                                                                                  Qt::CaseInsensitive)
        && !singleton<snpsettings>().map["ignorelist"].value<QStringList> ().contains(user,
                                                                                      Qt::CaseInsensitive)) {
        singleton<snpsettings>().map["buddylist"].setValue<QStringList> (
                singleton<snpsettings>().map["buddylist"].value<QStringList> () << user);
    }
    singleton<snpsettings>().safe();emit
            layoutChanged();
    emit dataChanged(createIndex(0, 0), createIndex(classes.count() - 1, 3));
}
void usermodel::deletebuddy(const QString &s) {

    emit
            layoutAboutToBeChanged();
    QStringList sl = singleton<snpsettings>().map["buddylist"].value<QStringList> ();
    QStringList::iterator i = sl.begin();
    QStringList temp;
    while (i != sl.end()) {
        if (compareCI(*i, s)) {
            temp << *i;
        }
        i++;
    }
    foreach(QString s,temp) {
        sl.removeAll(s);
    }
    singleton<snpsettings>().map["buddylist"] = sl;
    singleton<snpsettings>().safe();emit
            layoutChanged();
    emit dataChanged(createIndex(0, 0), createIndex(classes.count() - 1, 3));
}
void usermodel::addignore(const QString &s) {

    emit
            layoutAboutToBeChanged();
    if (!singleton<snpsettings>().map["ignorelist"].value<QStringList> ().contains(s,
                                                                                   Qt::CaseInsensitive)
        && !singleton<snpsettings>().map["buddylist"].value<QStringList> ().contains(s,
                                                                                     Qt::CaseInsensitive)) {
        singleton<snpsettings>().map["ignorelist"]
                = singleton<snpsettings>().map["ignorelist"].value<QStringList> () << s;
    }
    singleton<snpsettings>().safe();emit
            layoutChanged();
    emit dataChanged(createIndex(0, 0), createIndex(classes.count() - 1, 3));
}
void usermodel::deleteignore(const QString &s) {

    emit
            layoutAboutToBeChanged();
    userstruct u;
    u.nick = s;
    QStringList sl = singleton<snpsettings>().map["ignorelist"].value<QStringList> ();
    sl.removeOne(s);
    singleton<snpsettings>().map["ignorelist"] = sl;
    usermap[usermodel::tr("Ignorelist")].removeOne(u);
    singleton<snpsettings>().safe();emit
            layoutChanged();
    emit dataChanged(createIndex(0, 0), createIndex(classes.count() - 1, 3));
}
int usermodel::columnCount(const QModelIndex & /*parent*/) const {

    return 5;
}
int usermodel::rowCount(const QModelIndex & parent) const {

    if (!parent.isValid())
        return classes.count();
    if (parent.internalId() == e_Channel) {
        return usermap[classes[parent.row()]].count();
    }
    return 0;
}
QModelIndex usermodel::index(int row, int column, const QModelIndex & parent) const {

    if (users.isEmpty())
        return QModelIndex();
    if (!parent.isValid()) {
        return createIndex(row, column, e_Channel);
    }
    if (parent.internalId() == e_Channel) {
        return createIndex(row, column, parent.row());
    }
    return QModelIndex();
}
QModelIndex usermodel::parent(const QModelIndex & index) const {

    if (!index.isValid())
        return QModelIndex();
    if (index.internalId() == e_Channel)
        return QModelIndex();
    return createIndex(index.internalId(), 0, e_Channel);
}
QVariant usermodel::data(const QModelIndex & index, int role) const {

    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);

    if (role == Qt::DecorationRole) {
        if (index.internalId() == e_Channel && index.column() == e_Nick) {
            return channelicon;
        } else if (index.column() == e_Nick) {
            QString nick =usermap[classes[index.internalId()]][index.row()].nick;

            if (classes[index.internalId()] == tr("Querys") && !users.contains(
                    userstruct::whoami(nick)))
                return offlineicon;
            else if (singleton<snpsettings>().map["buddylist"].value<QStringList> ().contains(
                    nick, Qt::CaseInsensitive)) {
                if (containsCI(ctcphandler::awayusers, nick))
                    return awaybuddyicon;
                else
                    return buddyicon;
            } else if (singleton<snpsettings>().map["ignorelist"].value<QStringList> ().contains(
                    nick, Qt::CaseInsensitive)) {
                if (containsCI(ctcphandler::awayusers, nick))
                    return awayignoreicon;
                else
                    return ignoreicon;
            } else {
                if (containsCI(ctcphandler::awayusers, nick))
                    return awayusericon;
                else
                    return usericon;
            }
        } else
            return QVariant();
    }

    if (role == Qt::DisplayRole && index.internalId() != e_Channel && index.column() == e_Clan) {
        QString s = usermap[classes[index.internalId()]][index.row()].nickfromclient;
        if (singleton<snpsettings>().map["dissallowedclannames"].value<QStringList> ().contains(s,Qt::CaseInsensitive))
            return "";
        else
            return s;        
    }
    if (role == Qt::DisplayRole || role == Qt::BackgroundRole) {
        if (index.internalId() == e_Channel) {
            if (index.column() == e_Nick) {
                return classes[index.row()];
            } else
                return "";
        } else if (classes[index.internalId()] != tr("Buddylist")) {
            switch (index.column()) {
            case e_Nick:
                {
                    if (role == Qt::DisplayRole)
                        return usermap[classes[index.internalId()]][index.row()].nick;
                    break;
                }
            case e_Flag:
                {
                    if (role == Qt::BackgroundRole) {
                        return *flaglist[usermap[classes[index.internalId()]][index.row()].flag];
                    }
                    break;
                }
            case e_Rank:
                {
                    if (role == Qt::BackgroundRole) {
                        if(singleton<snpsettings>().map["tusloginenable"].toBool()){
                            static QString nick;
                            nick=usermap[classes[index.internalId()]][index.row()].nick;
                            int i=leagueserverhandler::map_at_toInt(nick,0);
                            if(i==-1)
                                return QVariant();
                            return *leagueranklist[i];
                        }
                        else
                            return *ranklist[usermap[classes[index.internalId()]][index.row()].rank];
                    }
                    break;
                }
            case e_Clan:
                {
                    if (role == Qt::DisplayRole)
                        return usermap[classes[index.internalId()]][index.row()].nickfromclient;
                    break;
                }
            case e_Client:
                {
                    if (role == Qt::DisplayRole)
                        return usermap[classes[index.internalId()]][index.row()].client.trimmed();
                    break;
                }
            }
        }//the buddylist may contain offline and online contacts... lets see
        else {
            switch (index.column()) {
            case e_Nick:
                {
                    if (role == Qt::DisplayRole)
                        return usermap[classes[index.internalId()]][index.row()].nick;
                    break;
                }
            case e_Flag:
                {
                    if (role == Qt::BackgroundRole) {
                        return *flaglist[usermap[classes[index.internalId()]][index.row()].flag];
                    }
                    break;
                }
            case e_Rank:
                {
                    if (role == Qt::BackgroundRole) {
                        return *ranklist[usermap[classes[index.internalId()]][index.row()].rank];
                    }
                    break;
                }
            case e_Clan:
                {
                    break;
                }
            case e_Client:
                {
                    if (role == Qt::DisplayRole)
                        return usermap[classes[index.internalId()]][index.row()].client;
                    break;
                }
            }
        }
    }
    if(role==Qt::FontRole && index.internalId() != e_Channel){
        if(singleton<settingswindow>().from_map("cbunderlinelinksandclans").toBool())
            return QVariant();
        if(index.column()==e_Client){
            QString s=usermap[classes[index.internalId()]][index.row()].client;
            if(isClickableLink(s)){
                QFont f;
                f.setUnderline(true);
                return f;
            }
        } if(index.column()==e_Clan){
            QString s=usermap[classes[index.internalId()]][index.row()].nickfromclient;
            if(singleton<clantowebpagemapper>().contains(s)){
                QFont f;
                f.setUnderline(true);
                return f;
            }
        }
    }    
    if(role==Qt::ToolTipRole && index.internalId() != e_Channel && index.column() == e_Clan){
        QString s=usermap[classes[index.internalId()]][index.row()].nickfromclient;
        return singleton<clantowebpagemapper>().getInformation(s);
    }
    return QVariant();
}
QVariant usermodel::headerData(int section, Qt::Orientation orientation,
                               int role) const {

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < 5) {
        return stringnamelist[section];
    }
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    if (role == Qt::FontRole)
        return QFont("Times", 12, QFont::Bold);

    return QVariant();
}
Qt::ItemFlags usermodel::flags(const QModelIndex & index) const {

    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.internalId() == e_Channel)
        return f ^= Qt::ItemIsSelectable;
    else if (index.column() == e_Flag || index.column() == e_Rank) {
        return f ^= Qt::ItemIsSelectable;
    } else
        return f;
}
//******************************for sorting**********************************
void usermodel::sortslot(int i, Qt::SortOrder s) {

    sortsection = i;
    sortorder = s;
}
unsigned int whichorder = 0;
bool operator<(const userstruct &u1, const userstruct &u2) {
    Q_ASSERT_X(whichorder<5,"whichorder","whichorder is greater then 4");
    switch (whichorder) {
    case 0:
        return u1.nick.toLower() < u2.nick.toLower();
    case 1:
        return u1.flag < u2.flag;
    case 2:
        return u1.rank < u2.rank;
    case 3:
        return u1.nickfromclient.toLower() < u2.nickfromclient.toLower();
    case 4:
        return u1.client.toLower() < u2.client.toLower();
    }
    return 0;
}
void usermodel::sort(int column, Qt::SortOrder order) {

    whichorder = column;
    if(!singleton<settingswindow>().from_map("cbdontsortinchannels").toBool()){
        if (order == Qt::AscendingOrder) {
            QMap<QString, QList<userstruct> >::iterator i = usermap.begin();
            while (i != usermap.end()) {
                qStableSort(i->begin(), i->end());
                i++;
            }
        } else if (order == Qt::DescendingOrder) {
            QMap<QString, QList<userstruct> >::iterator i = usermap.begin();
            while (i != usermap.end()) {
                qStableSort(i->begin(), i->end(), qGreater<userstruct> ());
                i++;
            }
        }
    }
    emit layoutChanged();
    emit dataChanged(createIndex(0, 0, e_Channel), createIndex(classes.count() - 1, 4, e_Channel));
}
//*************************sorting end*******************************

QModelIndex usermodel::indexbychannelname(QString s) {

    return createIndex(classes.indexOf(s), 0, e_Channel);
}
void usermodel::buddyarrived() {
    if (buddyarrivedhelper.size() > 3)
        buddyarrivedhelper.takeFirst();
    singleton<sound_handler>().play_buddyarrivedsound();
}
void usermodel::buddyleft() {    
    singleton<sound_handler>().play_buddyleftsound();
}
void usermodel::usesettingswindow(const QString&) {
}
userstruct usermodel::getuserstructbyindex(const QModelIndex &index) {
    userstruct u;
    u = users[users.indexOf(userstruct::whoami(data(index.sibling(index.row(),
                                                                  0), Qt::DisplayRole).value<QString> ()))];
    return u;
}
usermodel::~usermodel() {
}
