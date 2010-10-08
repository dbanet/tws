#include "about.h"
QString about::version="2.3";
about::about(QFrame *parent)
    : QFrame(parent)
{
	this->setObjectName("about");
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.textBrowser->setOpenExternalLinks(1);
	QString s(tr("<p>Version %1<br>"
			"This Program was completely written by Lookias.<br>"
			"It was written in C++, with the eclipse ide.<br>"
			"The framework in use was Qt.<br>"
			"The license of the program is GPLv2.<br>"
			"The translation was made by Lookias.<br>"
			"<br>"
                        "Lookias`s Homepage: <a href=\"http://lookias.worms2d.info/\">http://lookias.worms2d.info</a><br>"
                        "This programs livingplace <a href=\"http://lookias.worms2d.info/forum/\">http://lookias.worms2d.info/forum/<\a></p>").arg(version));
	ui.textBrowser->setHtml(s);
}
about::~about()
{

}
