//#ifndef SETTINGSHELPER_H
//#define SETTINGSHELPER_H
//#include<QSqlQuery>

//template <typename T>
//        class settingshelper
//{
//public:
//    settingshelper();
//    template <typename A>
//            void loadDefaults(A &a){
//        QSqlQuery query(QSqlDatabase::database(databasename));
//        query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
//        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
//        a.set("volumeslidervalue",5);
//        a.set("chbminimized", 0);
//        a.set("dissallowedclannames", QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
//        a.set("charformatfile", "comic by lookias.textscheme");
//        a.set("chbsendhostinfotochan", true);
//        a.set("language_file", "_en.Standard.qm");
//        installTranslationBySystemLocale();
//    }

//    template <typename A>
//            void validate(A &a){
//        checkifexistsinstringlist("leagueservers","http://www.tus-wa.com/");
//        checkifexistsinstringlist("leagueservers","http://www.normalnonoobs.org/");
//        checkifexistsinstringlist("leagueservers","http://lookias.worms2d.info/securelogging/");
//        checkifexistsinstringlist("wormnetserverlist","wormnet1.team17.com");
//        checkifexistsinstringlist("wormnetserverlist","itakagames.spb.ru");
//        checkifexistsinstringlist("wormnetserverlist","212.240.191.125");
//        checkifexistsinstringlist("wormnetserverlist","worms.tom.ru");
//        checkifexistsinstringlist("wormnetserverlist","http://steps.servegame.com");

//        if(getstring("rank").isEmpty())
//            a.set("rank", "13");
//        if(getstring("countrycode").isEmpty())
//            a.set("countrycode", "49");
//        if(getstring("qss_file").isEmpty())
//            a.set("qss_file", QString("black by lookias.qss"));
//        if(getstring("information").isEmpty() || getstring("information").startsWith("The Wheat Snooper"))
//            a.set("information", "The Wheat Snooper "+about::version);
//        if(!contains("showinformation"))
//            a.set("showinformation", true);
//        if(!contains("spectatingneversettedoff"))
//            a.set("spectatingneversettedoff", true);
//    }

//private:
//    template <typename A>
//            void checkifexistsinstringlist(QString key,QString value, A &a){
//        QStringList sl=getstringlist(key);
//        if(sl.contains(value,Qt::CaseInsensitive))
//            return;
//        sl<<value;
//        a.set(key, sl);
//    }
//};

//#endif // SETTINGSHELPER_H
