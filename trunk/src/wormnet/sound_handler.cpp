#include"sound_handler.h"
#include"settings.h"
#include"singleton.h"
#include"settingswindow.h"
#include"netcoupler.h"

#include<QApplication>
#include<phonon/phonon>
#include<QPointer>
#include<QMessageBox>

sound_handler::sound_handler(){    
    startupsound = new Phonon::MediaObject(this);   
    buddymsgsound = new Phonon::MediaObject(this);
    normalmsgsound = new Phonon::MediaObject(this);
    chatwindowopensound = new Phonon::MediaObject(this);
    highlightningsound = new Phonon::MediaObject(this);
    buddyhostedsound = new Phonon::MediaObject(this);
    costumwordsound=new Phonon::MediaObject(this);
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
    Phonon::createPath(costumwordsound, costumwordsoundsoundoutput);
}
void sound_handler::init(){
    startupsound->setCurrentSource(S_S.getstring("lestartup"));    
    normalmsgsound->setCurrentSource(S_S.getstring("lenormalchatmessage"));
    buddymsgsound->setCurrentSource(S_S.getstring("lebuddychatmessage"));
    chatwindowopensound->setCurrentSource(S_S.getstring("lebuddychatwindowsopened"));
    buddyleftsound->setCurrentSource(S_S.getstring("lebuddyleaves"));
    buddyarrivedsound->setCurrentSource(S_S.getstring("lebuddyarrives"));
    highlightningsound->setCurrentSource(S_S.getstring("lehighlightning"));
    buddyhostedsound->setCurrentSource(S_S.getstring("lehostsound"));
    costumwordsound->setCurrentSource(S_S.getstring("lecostumword"));

    volumechange(S_S.getint("volumeslidervalue"),false);
}
void sound_handler::volumechange(int i, bool b) {
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
void sound_handler::play_startupsound(){    
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
void sound_handler::play_buddyarrivedsound(){
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
void sound_handler::play_chatwindowopensound(){
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
void sound_handler::play_normalmsgsound(const QString user){
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
void sound_handler::play_buddymsgsound(const QString user){
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
void sound_handler::play_buddyleftsound(){
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
void sound_handler::play_highlightningsound(const QString user, QWidget *w){
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
void sound_handler::play_buddyhostedsound(){
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
void sound_handler::play_costumwordsound(const QString user, QWidget *w){
    if (!S_S.getbool("cbcostumword"))
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;    
    }
    costumwordsound->stop();
    costumwordsound->play();
    QApplication::alert(w);
}
void sound_handler::verify_if_played(const QString user) throw(dont_play_sound_exception){
    if(user.isEmpty())
        return;
    if(S_S.getbool("cbdontplaysound"))
        throw dont_play_sound_exception();    
    if (singleton<netcoupler>().mutedusers.contains(user, Qt::CaseInsensitive))
        throw dont_play_sound_exception();
}
sound_handler::~sound_handler(){
}
