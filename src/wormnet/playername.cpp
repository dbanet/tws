#include"playername.h"
#include"global_functions.h"

#include<QFile>
#include<QProcess>
#include<QTextStream>
QString playername::nick = "";
playername::playername(QWidget *parent) :
	QWidget(parent) {
    setObjectName("normalwidget");
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.label->setText(
            tr("This is your Playername in a game.\n"
               "This name supports more letters then the wormnet name.\n"
               "If this string is empty, worms will probably not host or join a game."));
    nick=get_winini_key ("PlayerName");
    ui.lineEdit->setText(nick);
    QRegExp rg;
    rg.setPattern (QString::fromWCharArray (L"(\\!|\"|#|\\$| |%|\\&|'|\\(|\\)|\\*|\\+|\\,|\\-|\\.|/|[0-9]|:|;|<|\\=|>|\\?|@|[A-Z]|\\[|\\\\|\\]|\\^|_|`|[a-z]|\\{|\\||\\}|Ö|ö|~|Ő|ő|Ű|ű|¿|À|Á|Â|Ã|Ä|Å|Æ|Ç|È|É|Ê|Ë|Ì|Í|Î|Ï|Ð|Ñ|Ò|Ó|Ô|Õ|×|Ø|Ù|Ú|Û|Ü|Ý|Þ|ß|à|á|â|ã|ä|å|æ|ç|è|é|ê|ë|ì|í|î|ï|ð|ñ|ò|ó|ô|õ|÷|ø|ù|ú|û|ü|ý|þ|ÿ|Б|Г|Д|Ж|З|И|Й|К|Л|П|У|Ф|Ц|Ч|Ш|Щ|Ъ|Ы|Ь|Э|Ю|X|Я|б|в|г|д|ж|з|и|й|Ÿ|¡|к|£|л|м|н|п|т|ф|ц|ч|ш|щ|ъ|ы|ь|э|ю|я){1,15}"));
    QRegExpValidator *validator = new QRegExpValidator(rg, 0);
    ui.lineEdit->setValidator (validator);
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
}
void playername::okclicked() {
    nick = ui.lineEdit->text ();
    if(!nick.isEmpty ())
        set_winini_key ("PlayerName",nick);
    close();
}
playername::~playername() {
}
