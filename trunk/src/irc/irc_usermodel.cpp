#include "irc_usermodel.h"
#include"src/wormnet/snpsettings.h"
#include"src/wormnet/settingswindow.h"
#include<QtGui>
#include<QDebug>
#include"src/irc/irc_netcoupler.h"
#include"src/wormnet/ctcphandler.h"
#include"src/wormnet/sound_handler.h"
#include"src/wormnet/global_functions.h"
#include"src/wormnet/usermodel.h"
#include "src/irc/irc_netcoupler.h"
extern QList<QPixmap*> flaglist; //declared in main.cpp
extern QList<QPixmap*> ranklist; //declared in main.cpp
extern QStringList querylist;
irc_usermodel::irc_usermodel(irc_netcoupler *net) :
        irc_net(net) {
    whichorder=0;
    stringnamelist << usermodel::tr("Nick") << "" << usermodel::tr("Rank") << usermodel::tr("Clan") << usermodel::tr("Information");

    sortorder = Qt::AscendingOrder;
    sortsection = 0;

    if (!channelicon.load("snppictures/channelicon.png"))
        qDebug() << "snppictures/channelicon.png is missing.";
    if (!usericon.load("snppictures/usericon.png"))
        qDebug() << "snppictures/usericon.png is missing.";
    if (!buddyicon.load("snppictures/buddyicon.png"))
        qDebug() << "snppictures/buddyicon.png is missing.";
    if (!ignoreicon.load("snppictures/ignoreicon.png"))
        qDebug() << "snppictures/ignoreicon.png is missing.";
    if (!offlineicon.load("snppictures/offlineicon.png"))
        qDebug() << "snppictures/offlineicon.png is missing.";
    if (!awayusericon.load("snppictures/awayusericon.png"))
        qDebug() << "snppictures/awayusericon.png is missing.";
    if (!awaybuddyicon.load("snppictures/awaybuddyicon.png"))
        qDebug() << "snppictures/awaybuddyicon.png is missing.";
    if (!awayignoreicon.load("snppictures/awayignoreicon.png"))
        qDebug() << "snppictures/awayignoreicon.png is missing.";
    currentselectedchannel = -1;
    usesettingswindow();
    //connect(irc_net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow())); ********in netcoupler
    connect(this, SIGNAL(sigbuddyleft()),this, SLOT(buddyleft()));
    connect(this, SIGNAL(sigbuddyarrived()),this, SLOT(buddyarrived()));
}
void irc_usermodel::selectionchanged(const QModelIndex &index, const QWidget *w) {

    selectionwidgetmap.remove(currentselecteduser);
    currentselecteduser = data(index.sibling(index.row(), 0)).value<QString> ();
    this->currentselectedchannel = index.internalId();
    selectionwidgetmap[currentselecteduser] = w;
}
void irc_usermodel::setuserstruct(QMap<QString,QList<userstruct> > joinlist) {

    emit layoutAboutToBeChanged();

    usermap.clear();
    foreach(QString s,joinlist.keys()){
        usermap[s]=joinlist[s];
    }
    QStringList sl;
    foreach(QString s,singleton<snpsettings>().map["buddylist"].value<QStringList>()) {
        if (irc_net->joinListContains(s)) {
            usermap[usermodel::tr("Buddylist")].push_back(userstruct::whoami(s));
            sl << s;
            if (currentbuddylist.removeAll(s) == 0) { //a buddy arrived
                usermodel::buddyarrivedhelper
                        << QTime::currentTime().toString("hh:mm") + ":" + s+usermodel::tr(" connected to wormnet.");
                emit sigbuddyarrived();
            }
        }
    }
    if (currentbuddylist.size() != 0){ //buddy left
        foreach(QString s,currentbuddylist){
            usermodel::buddylefthelper<< QTime::currentTime().toString("hh:mm") + ":" + s+usermodel::tr(" left wormnet.");
        }
        emit sigbuddyleft();
        foreach(QString s,currentbuddylist){
            removeCI(ctcphandler::awayusers,s);
        }
    }
    currentbuddylist = sl;
    foreach(QString s,querylist) {
        if (irc_net->joinListContains(s))
            usermap[usermodel::tr("Querys")].push_back(userstruct::whoami(s));
        else
            usermap[usermodel::tr("Querys")].push_back(userstruct(QStringList() << ""
                                                       << "" << "" << "" << s));
    }
    usermap[usermodel::tr("Buddylist")];
    usermap[usermodel::tr("Querys")];
    usermap[usermodel::tr("Ignorelist")];
    foreach(QString s,singleton<snpsettings>().map["ignorelist"].value<QStringList>()) {
        usermap[usermodel::tr("Ignorelist")].push_back(userstruct::whoami(s));
    }
    classes = usermap.keys();
    classes.move(classes.indexOf(usermodel::tr("Buddylist")), 0);
    classes.move(classes.indexOf(usermodel::tr("Querys")), 1);
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
void irc_usermodel::addbuddy(const QString &user) {

    emit layoutAboutToBeChanged();
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
void irc_usermodel::deletebuddy(const QString &s) {

    emit layoutAboutToBeChanged();
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
void irc_usermodel::addignore(const QString &s) {

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
void irc_usermodel::deleteignore(const QString &s) {

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
int irc_usermodel::columnCount(const QModelIndex & /*parent*/) const {

    return 5;
}
int irc_usermodel::rowCount(const QModelIndex & parent) const {

    if (!parent.isValid())
        return classes.count();
    if (parent.internalId() == e_Channel) {
        return usermap[classes[parent.row()]].count();
    }
    return 0;
}
QModelIndex irc_usermodel::index(int row, int column, const QModelIndex & parent) const {

    if (irc_net->countUsers()==0)
        return QModelIndex();
    if (!parent.isValid()) {
        return createIndex(row, column, e_Channel);
    }
    if (parent.internalId() == e_Channel) {
        return createIndex(row, column, parent.row());
    }
    return QModelIndex();
}
QModelIndex irc_usermodel::parent(const QModelIndex & index) const {

    if (!index.isValid())
        return QModelIndex();
    if (index.internalId() == e_Channel)
        return QModelIndex();
    return createIndex(index.internalId(), 0, e_Channel);
}
QVariant irc_usermodel::data(const QModelIndex & index, int role) const {    
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);

    if (role == Qt::DecorationRole) {
        if (index.internalId() == e_Channel && index.column() == e_Nick) {
            return channelicon;
        } else if (index.column() == e_Nick) {
            QString nick =usermap[classes[index.internalId()]][index.row()].nick;
            if (classes[index.internalId()] == usermodel::tr("Querys") && !irc_net->joinListContains(nick))
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
        } else if (classes[index.internalId()] != usermodel::tr("Buddylist")) {
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
    if(role==Qt::FontRole && index.column()==e_Client && index.internalId() != e_Channel){
        QString s=usermap[classes[index.internalId()]][index.row()].client;
        if(isClickableLink(s)){
            QFont f;
            f.setUnderline(true);
            return f;
        }
    }
    /*if(role==Qt::ToolTipRole && index.internalId() != e_Channel){
        if(index.column()==e_Clan) {
            QString s=usermodel::tr("The clan of this gamer is: ") + "\n" \
                         + usermap[classes[index.internalId()]][index.row()].nickfromclient  \
                         + "\n"+usermodel::tr("You can rightclick his clan\n to get further inforations about this clan.");
            return s;
        }
    }*/
    return QVariant();
}
QVariant irc_usermodel::headerData(int section, Qt::Orientation orientation,
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
Qt::ItemFlags irc_usermodel::flags(const QModelIndex & index) const {

    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.internalId() == e_Channel)
        return f ^= Qt::ItemIsSelectable;
    else if (index.column() == e_Flag || index.column() == e_Rank) {
        return f ^= Qt::ItemIsSelectable;
    } else
        return f;
}
//******************************for sorting**********************************
void irc_usermodel::sortslot(int i, Qt::SortOrder s) {

    sortsection = i;
    sortorder = s;
}
extern bool operator<(const userstruct &u1, const userstruct &u2);
void irc_usermodel::sort(int column, Qt::SortOrder order) {

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

QModelIndex irc_usermodel::indexbychannelname(QString s) {

    return createIndex(classes.indexOf(s), 0, e_Channel);
}
void irc_usermodel::buddyarrived() {
    if (usermodel::buddyarrivedhelper.size() > 3)
        usermodel::buddyarrivedhelper.takeFirst();
    singleton<sound_handler>().play_buddyarrivedsound();
}
void irc_usermodel::buddyleft() {
    singleton<sound_handler>().play_buddyleftsound();
}
void irc_usermodel::usesettingswindow(const QString&) {
}
userstruct irc_usermodel::getuserstructbyindex(const QModelIndex &index) {
    userstruct u;
//    u = users[users.indexOf(userstruct::whoami(data(index.sibling(index.row(),
//                                                                  0), Qt::DisplayRole).value<QString> ()))];
    return u;
}
irc_usermodel::~irc_usermodel() {
}
