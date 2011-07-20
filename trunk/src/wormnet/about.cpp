#include "about.h"
QString about::version="2.82";
about::about(QFrame *parent)
    : QFrame(parent)
{
    setObjectName("about");
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.textBrowser->setOpenExternalLinks(1);
    QString s=QString("<p>"+tr("Version")+" %1<br>" +
                      tr("This Program was completely written by Lookias.")+"<br>"+
                      tr("It was written in C++, with the eclipse ide.")+"<br>"+
                      tr("The framework in use was Qt.")+"<br>"+
                      tr("The license of the program is GPLv2.")+"<br>"+
                      tr("The translation was made by Lookias.")+"<br><br>"+
                      tr("Lookias`s Homepage:")+" <a href=\"http://lookias.worms2d.info/\">http://lookias.worms2d.info</a><br>"+
                      tr("This programs livingplace")+" <a href=\"http://lookias.worms2d.info/forum/\">http://lookias.worms2d.info/forum/<\a></p>").arg(version)+
            tr("<br><br>Participants:<br><br>")+
            "MihaiS<br>StepS<br>DENnis<br>Kawoosh<br>CyberShadow<br>MonkeyIsland<br>kilobyte<br>Killer<br>eViL<br>kosmos<br>LeTotalKiller<br>Domi<br>CzAjNiK<br>WowwoW<br> and many others :) ";

    ui.textBrowser->setHtml(s);
}
