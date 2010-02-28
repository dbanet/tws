#include "dropwidget.h"
#include <cmath>
#ifdef Q_WS_X11
//#include <kwindowsystem.h>
#endif
#define TARGET_SIZE 64
#define TARGET_ANI_MS 20
DropWidget::DropWidget(QWidget* parent /* = 0 */) //Konstruktor
: QWidget(parent)
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowTitle(tr("Drop widget"));
	this->setWindowIcon(QIcon("snppictures/buttons/upload.png"));
	sumFiles = 0;
	aniTimer = 0;
	setAttribute(Qt::WA_NoBackground);
	setAttribute(Qt::WA_NoSystemBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
#ifdef Q_WS_X11
	//KWindowSystem::setState(winId(), NET::SkipTaskbar);
#endif

	QRect desk = QRect(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());

	desk.setRight( desk.right() - TARGET_SIZE );
	desk.setBottom( desk.bottom() - TARGET_SIZE );
	position = QPoint((int)desk.width(), (int)desk.height() / 2);
	resize(TARGET_SIZE, TARGET_SIZE);
	widgetIcon = QPixmap(":/images/upload.png").scaled(TARGET_SIZE,TARGET_SIZE);

	if (!widgetIcon.mask().isNull())
	{
	    QBitmap mask(size());
	    mask.fill(Qt::color0);
	    QBitmap pixMask = widgetIcon.mask();
	    QPainter p(&mask);
	    p.drawPixmap((mask.width() - pixMask.width())/2, (mask.height() - pixMask.height())/2,
			pixMask);
	    setMask(mask);
	}
	else
	    setMask(QBitmap());
	widgetMask = widgetIcon;
	update();

	menu = new QMenu("Menu");
	menu->addAction(QIcon(":/images/send.png"),tr("Upload"), this, SLOT(emitUpload()));
	menu->addAction(QIcon(":/images/upload.png"), tr("Aniate"), this, SLOT(playAnimationShow()));
	menu->addAction(QIcon(":/images/exit.png"),tr("Close"), this, SLOT(close()));
	setAcceptDrops(true);


	playAnimationShow();
}

void DropWidget::playAnimationShow()
{
    if ( aniTimer )
        delete aniTimer;
    aniTimer = new QTimer;
    connect( aniTimer, SIGNAL( timeout() ),
        this, SLOT( slotAnimateShow() ));
    move(position.x(), -TARGET_SIZE);
    ani_y = -1;
    ani_vy = 0;
	aniTimer->start(TARGET_ANI_MS);
}

void DropWidget::playAnimationSync()
{
    if ( aniTimer )
    {
        if ( aniTimer->isActive() )
            move( x(), (int)(ani_y) );
        delete aniTimer;
    }
    aniTimer = new QTimer;
    connect( aniTimer, SIGNAL( timeout() ),
        this, SLOT( slotAnimateSync() ));
    ani_y = (float)y();
    ani_vy = -1;
    aniTimer->start(TARGET_ANI_MS);
}

void DropWidget::slotAnimateShow()
{
    static float dT = TARGET_ANI_MS / 1000.0;

    ani_vy -= ani_y * 30 * dT;
    ani_vy *= 0.95;
    ani_y += ani_vy * dT;

    move(x(), (int)(position.y() * (1 + ani_y)));

    if ( fabs(ani_y) < 0.01 && fabs(ani_vy) < 0.01 && aniTimer )
    {
        aniTimer->stop();
	aniTimer = 0;
    }

}

void DropWidget::slotAnimateSync()
{
    static float dT = TARGET_ANI_MS / 1000.0;

    ani_vy += 4 * dT;               // from -1 to 1 in 0.5 seconds
    float i = 2 * 3.14159265358979323846 * ani_vy;    // from -2PI to 2PI
    float j = (i == 0.0) ? 1 : (sin( i ) / i) * (1 + fabs(ani_vy));

    if ( ani_vy >= 1 )
    {
        aniTimer->stop();
        aniTimer = 0;
        move( x(), (int)(ani_y) );
    } else
        move( x(), (int)(ani_y + 6*j) );
}

void DropWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}
void DropWidget::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}
void DropWidget::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls())
	{
        QList<QUrl> urlList = mimeData->urls();
		foreach(QUrl url, urlList)
		{
	         QString text = url.toLocalFile();
			 path = QDir::cleanPath(text);
	         fileDroped(path);
		}
    }
	event->acceptProposedAction();
}
void DropWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}
void DropWidget::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton)
	 {
         dragPosition = event->globalPos() - frameGeometry().topLeft();
		 if(event->modifiers() == Qt::ControlModifier)
			emit wantsUpload();
         event->accept();
     }
	 if (event->button() == Qt::RightButton)
	 {
         menu->popup(event->globalPos());
         event->accept();
     }
 }

void DropWidget::mouseMoveEvent(QMouseEvent *event)
{
     if (event->buttons() & Qt::LeftButton)
	 {
         move(QCursor::pos() - dragPosition);
         event->accept();
     }
}

QSize DropWidget::sizeHint() const
{
     return QSize(widgetMask.width(), widgetMask.height());
}

void DropWidget::setProgress(qint64 done, qint64 total)
{
	double width = (double)done/(double)total * TARGET_SIZE;
	widgetMask.fill(Qt::transparent);
	QPainter p(&widgetMask);
	p.drawPixmap(0,0,widgetIcon);
	p.setPen(Qt::NoPen);
	p.setBrush(QColor(0,128,255,128));
	p.drawRect(QRect(0,0,width,TARGET_SIZE));
	p.end();
	if(done == total)
	  playAnimationSync();
	update();
}

void DropWidget::setFilesToUpload(int files)
{
	sumFiles = files;
	update();
}

void DropWidget::paintEvent(QPaintEvent */*event*/)
{
	QPainter p(this);
	p.fillRect(rect(), Qt::color0);
	QFont font("Arial", 32);
	font.setBold(true);
	p.setFont(font);
	p.setPen(QColor(255,255,255,128));
	p.drawPixmap(0,0,widgetMask);
	p.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(sumFiles));
}

void DropWidget::emitUpload()
{
	emit wantsUpload();
}

void DropWidget::closeEvent(QCloseEvent *e)
{
	e->ignore();
	hide();
}
