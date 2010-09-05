#include "about.h"
QString about::version="2.0";
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
			"Lookias`s Homepage: <a href=\"http://lookias.lo.funpic.de/page/index.htm\">lookias.lo.funpic.de/page</a><br>"
                        "This programs livingplace <a href=\"http://lookias.inventforum.com/viewforum.php?f=9\">lookias.inventforum.com<\a></p>").arg(version));
	ui.textBrowser->setHtml(s);
}

about::~about()
{

}
