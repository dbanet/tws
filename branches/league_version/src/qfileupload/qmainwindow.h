#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include "ui_qmainwindow.h"
#include "dropwidget.h"
#include "config.h"

#include <QtGui>
#include <QHttp>
#include <QFtp>
#include <QHostInfo>
#include <QHostAddress>



class MainWindow : public QWidget, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindow(QWidget* parent = 0); //Constructor
	static int counter;

	enum RegisterTransferLinkState
	{
		//Before Upload
		ConnectHttp,
		Server,
		ServerCheck,
		Size,
		SizeCheck,
		FileType,
		FileTypeCheck,
		FileId,
		FileIdCheck,
		//Upload
		ConnectFtp,
		Login,
		Port,
		Upload,
		CloseFtp,
		//After Upload
		Done,
		GetLink,
		GetLoesch,
		DoneCheck,
		CloseAll,
		Cancel,
		CloseHttpCancel,
		CloseHttp
	};

	void executeRegister();
	void executeUpload();
	void executeLinks();
	void executeCancel();

	QString formatByteText(qint64 bytes);
	qint64 calcSpeed(qint64 done);
public slots:
	void resetProgress();
	void saveSettings();
	void loadSettings();

	void startUpload();
	void uploadFinished();
	void httpRequestFinished( bool error);
	void commandFinished(int requestId, bool error);
	void addFile();
	void addFile(QString path);
	void delFile();
	void updateProgress( qint64 done, qint64 total);
	void changeWidget(QListWidgetItem *item);

	void saveVerlauf(QString dll, QString ll);
	void loadVerlauf();
	void delVerlauf();
protected:
	void closeEvent(QCloseEvent *e);
private:
	QString fileId;
	QString fileSize;
	QString fileServer;
	QString fileType;
	QString fileName;
	QString curPath;

	QString downloadLink;
	QString loeschLink;

	QFtp *ftp;
	QHttp *http;
	int state;
	int curState;
	bool isUploading;
	int doneLast;
	QString logText;

	DropWidget *dropWidget;
	ConfigDialog *confDialog;
	QSystemTrayIcon *trayIcon;
	QMenu *trayMenu;
	QColor linkColor, verlauf1Color, verlauf2Color, logColor, errColor;
};
#endif
