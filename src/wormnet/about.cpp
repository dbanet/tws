#include <QResource>
#include "about.h"

#define LOOKIAS_HOMEPAGE  "<a href='http://lookias.worms2d.info'>http://lookias.worms2d.info</a>"
#define OLD_TWS_FORUM     "<a href='http://lookias.worms2d.info/forum'>http://lookias.worms2d.info/forum/</a>"
#define NEW_TUS_TWS_FORUM "<a href='http://www.tus-wa.com/forums/worms-armageddon/new-the-wheat-snooper-2-9-community-build-24418'>http://www.tus-wa.com/forums/worms-armageddon/new-the-wheat-snooper-2-9-community-build-24418</a>"
#define GITHUB_PAGE       "<a href='http://github.com/dbanet/tws'>http://github.com/dbanet/tws</a>"

QString About::version=TWS_VERSION;
About::About(QFrame *parent)
    : QFrame(parent)
{
    setObjectName("about");
    setAttribute(Qt::WA_DeleteOnClose);

    ui.setupUi(this);
    ui.textBrowser->setOpenExternalLinks(1);

    QResource aboutHtmlRes("/about.html");
    QString aboutHtml=QString::fromUtf8((char*)aboutHtmlRes.data()).arg(
                TWS_VERSION,
                LOOKIAS_HOMEPAGE,
                OLD_TWS_FORUM,
                NEW_TUS_TWS_FORUM,
                GITHUB_PAGE
    );

    ui.textBrowser->setHtml(aboutHtml);
}
