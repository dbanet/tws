#include "qfileupload/qmainwindow.h"

//General file-upload Data
#define WWW		"www"
#define FTPHOST 	"file-upload.net"
#define HTTPHOST 	"file-upload.net"
#define PHPSCRIPT 	"/connector/connect.php?"
#define USER 		"ftptool"
#define PASSWORD 	"hi68GHf5"
#define SUFFIX 		".upload"

//Before FTP Upload
#define OP_SERVER 		"op=server"
#define OP_SERVER_CHECK 	"op=server_check&id=%id%"
#define OP_SIZE 		"op=size"
#define OP_SIZE_CHECK		"op=size_check&size=%size%"
#define OP_FILE_TYPE		"op=file_type"
#define OP_FILE_TYPE_CHECK	"op=file_type_check&type=%type%"
#define OP_FILE_ID		"op=file_id&name=%name%"
#define OP_FILE_ID_CHECK	"op=file_id_check&id=%id%"
//After Upload
#define OP_DONE			"op=done&id=%id%"
#define OP_GET_LINK		"op=get_link&id=%id%"
#define OP_GET_LOESCH		"op=get_loesch_link&id=%id%"
#define OP_DONE_CHECK		"op=done_check&id=%id%"
#define OP_CANCEL			"op=cancel&id=%id%"
int MainWindow::counter=0;
MainWindow::MainWindow(QWidget* parent /* = 0 */) //Konstruktor
: QWidget(parent)
{
	counter++;
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	isUploading = false;

	loadSettings();
	loadVerlauf();
	confDialog = new ConfigDialog(container);
	confDialog->init(linkColor, errColor, logColor, verlauf1Color, verlauf2Color);
	confDialog->hide();
	container->layout()->addWidget(confDialog);
	logWidget->hide();
	verlaufWidget->hide();
	aboutWidget->hide();
	resize(450, 300);

	dropWidget = new DropWidget;
	dropWidget->show();

	trayIcon = new QSystemTrayIcon(QIcon(":/images/upload.png"), this);
	trayMenu = new QMenu("trayMenu");
	trayMenu->addAction(QIcon(":/images/diag.png"),tr("Mainwindow"), this, SLOT(show()));
	QAction *action = trayMenu->addAction(QIcon(":/images/upload.png"),tr("Drop widget"), dropWidget, SLOT(show()));
	connect(action,SIGNAL(triggered()), dropWidget, SLOT(raise()));
	trayMenu->addAction(QIcon(":/images/send.png"), tr("Upload"), this, SLOT(startUpload()));
	trayMenu->addAction(QIcon(":/images/exit.png"),tr("Close"), this, SLOT(close()));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	http = new QHttp(this);
	ftp = new QFtp(this);
	fileServer = "www";
	connect(http, SIGNAL(done( bool)),
             this, SLOT(httpRequestFinished(bool)));

	connect(ftp, SIGNAL(commandFinished(int, bool)),
             this, SLOT(commandFinished(int, bool)));

	connect(ftp, SIGNAL(dataTransferProgress ( qint64 , qint64  )),
             this, SLOT(updateProgress ( qint64 , qint64  )));

	connect(buttonStart, SIGNAL(clicked()),
             this, SLOT(startUpload()));

	connect(addButton, SIGNAL(clicked()),
             this, SLOT(addFile()));

	connect(delButton, SIGNAL(clicked()),
             this, SLOT(delFile()));

	connect(delHistoryButton, SIGNAL(clicked()),
             this, SLOT(delVerlauf()));

	connect(widgetList, SIGNAL(itemClicked(QListWidgetItem *)),
             this, SLOT(changeWidget(QListWidgetItem *)));

	connect(dropWidget, SIGNAL(wantsUpload()),
             this, SLOT(startUpload()));

	connect(dropWidget, SIGNAL(fileDroped(QString)),
             this, SLOT(addFile(QString)));

	connect(confDialog, SIGNAL(save()),
             this, SLOT(saveSettings()));

	QApplication::setQuitOnLastWindowClosed(false);
}


void MainWindow::saveSettings()
{
	linkColor = confDialog->link;
	logColor = confDialog->log;
	errColor = confDialog->err;
	verlauf1Color = confDialog->verlauf1;
	verlauf2Color = confDialog->verlauf2;
	QSettings standard(QApplication::applicationDirPath() + "/qfileupload.conf", QSettings::IniFormat);
	standard.beginGroup("Colors");
	standard.setValue(tr("Link"), linkColor.name());
	standard.setValue(tr("Log"), logColor.name());
	standard.setValue(tr("Error"), errColor.name());
	standard.setValue(tr("History1"), verlauf1Color.name());
	standard.setValue(tr("History2"), verlauf2Color.name());
	standard.endGroup();
}

void MainWindow::loadSettings()
{
	QSettings standard(QApplication::applicationDirPath() + "/qfileupload.conf", QSettings::IniFormat);
	standard.beginGroup("Colors");
	linkColor = QColor(standard.value("Link", QColor(0,0,255).name()).toString());
	logColor = QColor(standard.value("Log", QColor(0,0,0).name()).toString());
	errColor = QColor(standard.value("Error", QColor(255,0,0).name()).toString());
	verlauf1Color = QColor(standard.value("History1", QColor(255,0,0).name()).toString());
	verlauf2Color = QColor(standard.value("History2", QColor(100,100,100).name()).toString());
	standard.endGroup();
}

void MainWindow::changeWidget(QListWidgetItem *item)
{
  logWidget->hide();
  verlaufWidget->hide();
  uploadWidget->hide();
  aboutWidget->hide();
  confDialog->hide();
  if(item->text() == tr("Upload"))
     uploadWidget->show();
  else if(item->text() == tr("Log"))
     logWidget->show();
  else if(item->text() == tr("Hystorie"))
     verlaufWidget->show();
  else if(item->text() == tr("About"))
     aboutWidget->show();
  else if(item->text() == tr("Settings"))
     confDialog->show();
}

void MainWindow::addFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Load File (maximum 100mb)"),

													 QApplication::applicationDirPath());
	if(!filePath.isEmpty())
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(":/images/file.png"), filePath);
		fileWidget->addItem(item);
		dropWidget->setFilesToUpload(fileWidget->count());
	}
}

void MainWindow::addFile(QString path)
{

	if(!path.isEmpty())
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(":/images/file.png"), path);
		fileWidget->addItem(item);
		dropWidget->setFilesToUpload(fileWidget->count());
	}
}

void MainWindow::delFile()
{
	if(fileWidget->count() > 0)
		fileWidget->takeItem(fileWidget->currentRow());
	dropWidget->setFilesToUpload(fileWidget->count());
}

void MainWindow::startUpload()
{
	if(isUploading)
	{
		state = CloseAll;
		executeCancel();
	}
	else
	{
		if(fileWidget->count() > 0)
		{
		  isUploading = true;
		  buttonStart->setIcon(QIcon(":/images/exit.png"));
		  buttonStart->setText(tr("Cancel"));
		  state = ConnectHttp;
		  curPath = fileWidget->item(0)->text();
                  fileName = QFileInfo(curPath).fileName();
		  executeRegister();
		}
		else
		{
		  QMessageBox::information(this, tr("Upload"), tr("No files to upload."));
		}
	}
}

void MainWindow::uploadFinished()
{
	buttonStart->setIcon(QIcon(":/images/send.png"));
	buttonStart->setText(tr("Send"));
	isUploading = false;
	saveVerlauf(downloadLink, loeschLink);
	fileWidget->takeItem(0);
	dropWidget->setFilesToUpload(fileWidget->count());
	loadVerlauf();
	if(fileWidget->count() > 0)
		startUpload();

}

void MainWindow::executeRegister()
{
	//qDebug("register the file");
	switch(state)
	{
		case ConnectHttp:
			logText += "<font color="+ logColor.name() +">" + tr("Start Upload") + ".</font><br>";
			logNew->setHtml(logText);
			curState = ConnectHttp;
			http->setHost(QString(WWW) + "." + QString(HTTPHOST));
			state = Server;
		break;

		case Server:
			curState = Server;
			http->get(PHPSCRIPT + QString(OP_SERVER));
			state = ServerCheck;
		break;

		case ServerCheck:
			curState = ServerCheck;
			http->get(PHPSCRIPT + QString(OP_SERVER_CHECK).replace("%id%", fileServer));
			state = Size;
		break;

		case Size:
			curState = Size;
			http->get(PHPSCRIPT + QString(OP_SIZE));
			state = SizeCheck;
		break;

		case SizeCheck:
			curState = SizeCheck;
			http->get(PHPSCRIPT + QString(OP_SIZE_CHECK).replace("%size%", fileSize));
			state = FileType;
		break;

		case FileType:
			curState = FileType;
			http->get(PHPSCRIPT + QString(OP_FILE_TYPE));
			state = FileTypeCheck;
		break;

		case FileTypeCheck:
			curState = FileTypeCheck;
			http->get(PHPSCRIPT + QString(OP_FILE_TYPE_CHECK).replace("%type%", fileType));
			state = FileId;
		break;

		case FileId:
			curState = FileId;
			http->get(PHPSCRIPT + QString(OP_FILE_ID).replace("%name%", fileName));
			state = FileIdCheck;
		break;

		case FileIdCheck:
			curState = FileIdCheck;
			http->get(PHPSCRIPT + QString(OP_FILE_ID_CHECK).replace("%id%", fileId));
			state = ConnectFtp;
		break;

		default:
		    curState = ConnectHttp;
		    state = ConnectHttp;
		break;
	}
}

void MainWindow::executeUpload()
{

	QFile ufile(curPath);
	QByteArray dat;
	//qDebug("upload the file");
	switch(state)
	{
		case ConnectFtp:
			curState = ConnectFtp;

			logText += "<font color=\""+ logColor.name() +"\">" + tr("Upload file  \t: ") + fileName + ".</font><br>";
			logNew->setHtml(logText);
			ftp->connectToHost(fileServer +  + "." + QString(FTPHOST));
			state = Login;
		break;

		case Login:
			curState = Login;
			ftp->login(USER, PASSWORD);
			state = Port;
		break;

		case Port:
			curState = Port;
			ftp->rawCommand("PORT");
			state = Upload;
		break;

		case Upload:
			curState = Upload;
			ufile.open(QIODevice::ReadOnly);
			dat = ufile.readAll();
			ufile.close();

			ftp->put(dat, fileId + SUFFIX, QFtp::Binary);
			state = CloseFtp;
		break;

		case CloseFtp:
			logText += "<font color=\""+ logColor.name() +"\">" + tr("Finish upload \t: ") + fileName + " .</font><br>";
			logNew->setHtml(logText);
			curState = CloseFtp;
			ftp->close();
			state = Done;
		break;
		default:
		    state = ConnectHttp;
		break;
	}
}

void MainWindow::executeLinks()
{
	switch(state)
	{
		case Done:
			curState = Done;
			http->get(PHPSCRIPT + QString(OP_DONE).replace("%id%", fileId));
			state = GetLink;
		break;

		case GetLink:
			curState = GetLink;
			http->get(PHPSCRIPT + QString(OP_GET_LINK).replace("%id%", fileId));
			state = GetLoesch;
		break;

		case GetLoesch:
			curState = GetLoesch;
			http->get(PHPSCRIPT + QString(OP_GET_LOESCH).replace("%id%", fileId));
			state = DoneCheck;
		break;

		case DoneCheck:
			curState = DoneCheck;
			http->get(PHPSCRIPT + QString(OP_DONE_CHECK).replace("%id%", fileId));
			state = CloseHttp;
		break;

		case CloseHttp:
			curState = CloseHttp;
			http->close();
			state = ConnectHttp;
			logText += "<font color=\""+ logColor.name() +"\">" + tr("Links for \t: ") + fileName + ".</font><br>";
			logText += "<a style='color:" + linkColor.name() + "' href=" + downloadLink + ">DownloadLink.</a><br>";
			logText += "<a style='color:" + linkColor.name() + "' href=" + loeschLink + tr(">DeleteLink.</a><br>");
			logNew->setHtml(logText);
			uploadFinished();
		break;

		default:
		    state = ConnectHttp;
		break;
	}
}

void MainWindow::executeCancel()
{
	switch(state)
	{
		case CloseAll:
			ftp->abort();
			http->abort();
			http->close();
			ftp->close();
			curState = CloseAll;
			http->setHost(QString(WWW)  + "." + QString(HTTPHOST));
			state = Cancel;
		break;

		case Cancel:
			curState = Cancel;
			http->get(PHPSCRIPT + QString(OP_CANCEL).replace("%id%", fileId));
			state = CloseHttpCancel;
		break;

		case CloseHttpCancel:
			curState = CloseHttpCancel;
			http->close();
			state = ConnectHttp;

			logText += "<font color=\""+ errColor.name() +"\">" + tr("Upload canceled") + ".</font><br>";
			logNew->setHtml(logText);
			isUploading = false;
			buttonStart->setIcon(QIcon(":/images/send.png"));
			buttonStart->setText(tr("Send"));
			resetProgress();
		break;

		default:
		    state = ConnectHttp;
		break;
	}
}

void MainWindow::httpRequestFinished( bool error)
{
	//qDebug("state : " + QByteArray::number(curState));
	if (!error)
	{
		bool err = false;
		int execute = -1;
		QString returnString = "OK";
		switch(curState)
		{
		  case ConnectHttp:
			execute = 0;
		  break;
		  case Server:
		    fileServer = http->readAll();
			execute = 0;
		  break;
		  case ServerCheck:
			returnString = http->readAll();
			execute = 0;
		  break;
		  case Size:
		    fileSize = http->readAll();
			execute = 0;
		  break;
		  case SizeCheck:
		      returnString = http->readAll();
			execute = 0;
		  break;
		  case FileType:
		    fileType = http->readAll();
			execute = 0;
		  break;
		  case FileTypeCheck:
		      returnString = http->readAll();
			execute = 0;
		  break;
		  case FileId:
		    fileId = http->readAll();
			execute = 0;
		  break;
		  case FileIdCheck:
		      returnString = http->readAll();
				execute = 1;
		  break;
		  case Done:
		      returnString = http->readAll();
			execute = 2;
		  break;
		  case GetLink:
		    downloadLink = http->readAll();
			execute = 2;
		  break;
		  case GetLoesch:
		    loeschLink = http->readAll();
		    execute = 2;
		  break;
		  case DoneCheck:
		      returnString = http->readAll();
			execute = 2;
		  break;
		  case CloseAll:
			execute = 3;
		  break;
		  case Cancel:
		    returnString = http->readAll();
			execute = 3;
		  break;
		  case CloseHttpCancel:
		  break;
		  case CloseHttp:
		  break;
		}
		if(!returnString.contains("OK"))
		    err = false;
		if(!err)
		{
			switch(execute)
			{
				case 0:
					executeRegister();
				break;
				case 1:
					executeUpload();
				break;
				case 2:
					executeLinks();
				break;
				case 3:
					executeCancel();
				break;
			}
		}
		else
		{
		  QMessageBox::information(this, tr("Error"), tr("One transmission was erronius\n Cancel"));
			state = CloseAll;
		  executeCancel();
		}
	}
	else
	{
		logText += "<font color=\""+ errColor.name() +"\">" + tr("Error : ") + http->errorString() +  "</font><br>";
		logNew->setHtml(logText);
	}
 }
void MainWindow::commandFinished(int requestId, bool error)
{

	//qDebug("state : " + QByteArray::number(curState));
	if (!error)
	{
		if(curState < CloseFtp)
		  executeUpload();
		else
		  executeLinks();
	}
	else
	{

		logText += "<font color=\""+ errColor.name() +"\">" + tr("Error : ") + QString::number(requestId) + " : " + ftp->errorString() +  "</font><br>";
		logNew->setHtml(logText);
	}
 }

QString MainWindow::formatByteText(qint64 bytes)
{
	QString kmbytes;
	double tmp = bytes;
	if(tmp > 500)
	{
		tmp /= 1000;
		kmbytes = " kB";
	}
	if(tmp > 500)
	{
		tmp /= 1000;
		kmbytes = " MB";
	}

	kmbytes.prepend(QString::number(tmp));
	return kmbytes;
}

qint64 MainWindow::calcSpeed(qint64 done)
{
  qint64 speed = 0;
  speed = done - doneLast;
  doneLast = done;
  return speed/2000;
}

void MainWindow::updateProgress( qint64 done, qint64 total)
{
	dropWidget->setProgress(done,total);
	pBar->setMaximum(total);
	pBar->setValue(done);
	pBar->setFormat(formatByteText(done).toAscii() + " / " + formatByteText(total).toAscii()
			+ "\n" + QString::number(calcSpeed(done)) + " KB/s");
	if(done == total)
	  QTimer::singleShot(3000, this, SLOT(resetProgress()));
}

void MainWindow::resetProgress()
{
  dropWidget->setProgress(0,1);
  updateProgress(0,1);
  doneLast = 0;
}

void MainWindow::saveVerlauf(QString dll, QString ll)
{
	QFile file(QApplication::applicationDirPath() + "/uploads.dat");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	file.write(fileName.toAscii() + "\n" +
			   dll.toAscii() + "\n" +
			   ll.toAscii() + "\n\n");
	file.close();
}

void MainWindow::loadVerlauf()
{
	QString html;
	QString dll, ll, fn;
	QFile file(QApplication::applicationDirPath() + "/uploads.dat");
	file.open(QIODevice::ReadOnly);
	while(!file.atEnd())
	{
		fn = file.readLine();
		dll = file.readLine();
		ll = file.readLine();
		file.readLine();
		html += "<font color=\""+ verlauf1Color.name() +"\">" + fn +  "</font><br>";
		html += "<a style='color:" + linkColor.name() + "' href=" + dll + tr(">DownloadLink.</a><br>");
		html += "<a style='color:" + linkColor.name() + "' href=" + ll + tr(">DeleteLink.</a><br>");

		html += "<br>";
	}
	logOld->setHtml(html);
	file.close();
}

void MainWindow::delVerlauf()
{
	QFile file(QApplication::applicationDirPath() + "/uploads.dat");
	file.open(QIODevice::WriteOnly);
	file.write("");
	file.close();
	loadVerlauf();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	counter--;
	this->dropWidget->deleteLater();
	this->deleteLater();

	e->ignore();
	hide();
}


