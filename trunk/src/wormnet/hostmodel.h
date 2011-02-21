#ifndef HOSTMODEL_H
#define HOSTMODEL_H
#include<QAbstractItemModel>
#include<QPixmap>
#include<QStringList>
#include"hoststruct.h"

class hostmodel: public QAbstractItemModel {
Q_OBJECT

public:
	hostmodel(QObject *parent = 0);
	~hostmodel();

	void sethoststruct(QList<hoststruct> l,QString channel);
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role =Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex & parent =QModelIndex()) const;
	QModelIndex parent(const QModelIndex & index) const;
	Qt::ItemFlags flags(const QModelIndex & index) const;

	QModelIndex indexbychannelname(QString);

	QString joininfo(const QModelIndex &);
	QString gamename(const QModelIndex &index);
        QHash<QString,QList<hoststruct> > hostmap;
	QStringList classes;
private:
	QStringList stringnamelist;
	QPixmap channelicon;
	QPixmap hosticon;
	QPixmap buddyhosticon;
	QPixmap ignorehosticon;

        QHash<QString,QStringList> buddyhosts;
};

#endif // HOSTMODEL_H
