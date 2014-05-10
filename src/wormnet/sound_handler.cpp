#include "sound_handler.h"
#include "settings.h"
#include "singleton.h"
#include "settingswindow.h"

#include <QApplication>
#include <phonon/phonon>
#include <QPointer>
#include <QMessageBox>

soundHandler::soundHandler(){
    startupsound = new Phonon::MediaObject(this);
    buddymsgsound = new Phonon::MediaObject(this);
    normalmsgsound = new Phonon::MediaObject(this);
    chatwindowopensound = new Phonon::MediaObject(this);
    highlightningsound = new Phonon::MediaObject(this);
    buddyhostedsound = new Phonon::MediaObject(this);
    customwordsound=new Phonon::MediaObject(this);
    buddyleftsound = new Phonon::MediaObject(this);
    buddyarrivedsound = new Phonon::MediaObject(this);

    chatwindowopensoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    normalmsgsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddymsgsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    startupsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    highlightningoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddyleftsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddyarrivedsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddyhostedsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    costumwordsoundsoundoutput= new Phonon::AudioOutput(Phonon::MusicCategory);

    Phonon::createPath(startupsound, startupsoundoutput);

    Phonon::createPath(chatwindowopensound, chatwindowopensoundoutput);
    Phonon::createPath(buddymsgsound, buddymsgsoundoutput);
    Phonon::createPath(normalmsgsound, normalmsgsoundoutput);
    Phonon::createPath(buddyarrivedsound, buddyarrivedsoundoutput);
    Phonon::createPath(buddyleftsound, buddyleftsoundoutput);
    Phonon::createPath(highlightningsound, highlightningoutput);
    Phonon::createPath(buddyhostedsound, buddyhostedsoundoutput);
    Phonon::createPath(customwordsound, costumwordsoundsoundoutput);
}
void soundHandler::init(){
    startupsound->setCurrentSource(S_S.getString("lestartup"));
    normalmsgsound->setCurrentSource(S_S.getString("lenormalchatmessage"));
    buddymsgsound->setCurrentSource(S_S.getString("lebuddychatmessage"));
    chatwindowopensound->setCurrentSource(S_S.getString("lebuddychatwindowsopened"));
    buddyleftsound->setCurrentSource(S_S.getString("lebuddyleaves"));
    buddyarrivedsound->setCurrentSource(S_S.getString("lebuddyarrives"));
    highlightningsound->setCurrentSource(S_S.getString("lehighlightning"));
    buddyhostedsound->setCurrentSource(S_S.getString("lehostsound"));
    customwordsound->setCurrentSource(S_S.getString("lecostumword"));

    volumechange(S_S.getint("volumeslidervalue"),false);
}
void soundHandler::volumechange(int i, bool b) {
    startupsoundoutput->setVolume(double(i) / 10);
    chatwindowopensoundoutput->setVolume(double(i) / 10);
    normalmsgsoundoutput->setVolume(double(i) / 10);
    chatwindowopensoundoutput->setVolume(double(i) / 10);
    buddyarrivedsoundoutput->setVolume(double(i) / 10);
    buddyleftsoundoutput->setVolume(double(i) / 10);
    costumwordsoundsoundoutput->setVolume(double(i) / 10);
    if(b)
        S_S.set("volumeslidervalue", i);
}
void soundHandler::play_startupsound(){
    if (!S_S.getbool("cbstartup"))
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    startupsound->stop();
    startupsound->play();
}
void soundHandler::play_buddyarrivedsound(){
    if (!S_S.getbool("cbbuddyarrives"))
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    buddyarrivedsound->stop();
    buddyarrivedsound->play();
}
void soundHandler::play_chatwindowopensound(){
    if (!S_S.getbool("cbplaybuddychatwindowopened"))
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    chatwindowopensound->stop();
    chatwindowopensound->play();
}
void soundHandler::play_normalmsgsound(const QString user){
    if (!S_S.getbool("cbplaynormalchatmessage"))
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;
    }
    normalmsgsound->stop();
    normalmsgsound->play();
}
void soundHandler::play_buddymsgsound(const QString user){
    if (!S_S.getbool("cbplaybuddychatmessage"))
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;
    }
    buddymsgsound->stop();
    buddymsgsound->play();
}
void soundHandler::play_buddyleftsound(){
    if (!S_S.getbool("cbbuddyleaves"))
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    buddyleftsound->play();
    buddyleftsound->play();
}
void soundHandler::play_highlightningsound(const QString user, QWidget *w){
    Q_ASSERT(w != NULL);
    if (!S_S.getbool("cbhighlightning"))
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;
    }
    highlightningsound->stop();
    highlightningsound->play();
    QApplication::alert(w);
}
void soundHandler::play_buddyhostedsound(){
    if (!S_S.getbool("chbhostsound"))
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    buddyhostedsound->stop();
    buddyhostedsound->play();
}
void soundHandler::play_costumwordsound(const QString user, QWidget *w){
    Q_ASSERT(w != NULL);
    if (!S_S.getbool("cbcostumword"))
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;    
    }
    customwordsound->stop();
    customwordsound->play();
    QApplication::alert(w);
}
void soundHandler::verify_if_played(const QString user) throw(dont_play_sound_exception){
    if(user.isEmpty())
        return;
    if(S_S.getbool("cbdontplaysound"))
        throw dont_play_sound_exception();    
    if (S_S.getStringList("mutedusers").contains(user, Qt::CaseInsensitive)
    || (S_S.ignorelist.contains(user, Qt::CaseInsensitive)))
        throw dont_play_sound_exception();
}
soundHandler::~soundHandler(){
}
