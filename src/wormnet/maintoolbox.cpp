#include "maintoolbox.h"
#include"window.h"
#include"chatwindow.h"
#include"mytabwidget.h"
#include<QVBoxLayout>
maintoolbox::maintoolbox(QString s,QWidget *parent)
    : QWidget(parent)
{
	 setObjectName("normalwidget");
	 setWindowTitle(s);
	layout=new QVBoxLayout;
	tab=new mytabwidget;
	layout->addWidget(tab);
	tab->clear();
	 setLayout(layout);
	connect(tab,SIGNAL(sigclosetab(int)),this,SLOT(closewindow(int)));
}
void maintoolbox::addwidget(QWidget *w){
	widgetlist.push_back(w);
	tab->addTab(w,w->windowIcon(),w->windowTitle());
}
void maintoolbox::removeallwidgets(){
	widgetlist.clear();
	QWidget *w;
	for(int i=0;i<tab->count();++i){
		w=tab->widget(i);
		w->setParent(0);
	}
	tab->clear();
}
void maintoolbox::closewindow(int i){
	tab->removeTab(i);
	emit sigclosewindow(widgetlist[i]);
	widgetlist.removeAt(i);
	if(tab->count()==0)
		 hide();
}
void maintoolbox::activateone(QWidget*){

}
void maintoolbox::alert(QWidget *w){
	int i=tab->indexOf(w);
	if(i!=-1 && tab->currentIndex()!=i){
		tab->setTabIcon(i,QIcon());
	}
}
maintoolbox::~maintoolbox()
{
}
