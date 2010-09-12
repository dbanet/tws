#include "volumeslider.h"

volumeslider::volumeslider(QFrame *parent)
    :QFrame(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(tr("Set the global volume."));
	this->setObjectName("volumeslider");
	ui.horizontalSlider->setTracking(0);
	connect(ui.horizontalSlider,SIGNAL(valueChanged(int)),this,SIGNAL(valueChanged(int)));
}
double volumeslider::value(){
	return ui.horizontalSlider->value();
}
void volumeslider::setvalue(int i){
	ui.horizontalSlider->setValue(i);
}
volumeslider::~volumeslider()
{

}
