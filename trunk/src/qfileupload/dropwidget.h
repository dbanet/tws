#ifndef DROPWIDGET_H_
#define DROPWIDGET_H_

#include <QtGui>


class DropWidget : public QWidget
{
Q_OBJECT
public:
	DropWidget(QWidget* parent = 0); //Constructor
	~DropWidget(){}
    QSize sizeHint() const;
public slots:
	void emitUpload();
	void slotAnimateShow();
	void playAnimationShow();
	void slotAnimateSync();
	void playAnimationSync();
	void setProgress(qint64 done, qint64 total);
	void setFilesToUpload(int files);
signals:
	void fileDroped(QString path);
	void wantsUpload();
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
	void closeEvent(QCloseEvent *e);
private:
	QPixmap widgetMask;
	QPixmap widgetIcon;
	QPoint dragPosition;
	QString path;
	QMenu *menu;

	QTimer *aniTimer;
	float ani_y, ani_vy;
	QPoint position;
	int sumFiles;
};
#endif
