#include "sound_handler.h"
#include "snpsettings.h"
#include "singleton.h"
#include"settingswindow.h"
#include"netcoupler.h"
#include<phonon/phonon>
#include<QPointer>
#include<QMessageBox>
extern QPointer<netcoupler> net;
sound_handler::sound_handler(){
    startupsound = new Phonon::MediaObject(this);
    chatwindowopensoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    normalmsgsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddymsgsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddymsgsound = new Phonon::MediaObject(this);
    normalmsgsound = new Phonon::MediaObject(this);
    chatwindowopensound = new Phonon::MediaObject(this);
    highlightningsound = new Phonon::MediaObject(this);
    buddyhostedsound = new Phonon::MediaObject(this);
    costumwordsound=new Phonon::MediaObject(this);

    startupsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    highlightningoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddyleftsoundoutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    buddyleftsound = new Phonon::MediaObject(this);
    buddyarrivedsound = new Phonon::MediaObject(this);
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
    startupsound->setCurrentSource(
            singleton<settingswindow>().from_map("lestartup").value<QString> ());    
    normalmsgsound->setCurrentSource(
            singleton<settingswindow>().from_map("lenormalchatmessage").value<QString> ());
    buddymsgsound->setCurrentSource(
            singleton<settingswindow>().from_map("lebuddychatmessage").value<QString> ());
    chatwindowopensound->setCurrentSource(
            singleton<settingswindow>().from_map("lebuddychatwindowsopened").value<QString> ());
    buddyleftsound->setCurrentSource(
            singleton<settingswindow>().from_map("lebuddyleaves").value<QString> ());
    buddyarrivedsound->setCurrentSource(
            singleton<settingswindow>().from_map("lebuddyarrives").value<QString> ());
    highlightningsound->setCurrentSource(
            singleton<settingswindow>().from_map("lehighlightning").value<QString> ());
    buddyhostedsound->setCurrentSource(
            singleton<settingswindow>().from_map("lehostsound").value<QString> ());
    costumwordsound->setCurrentSource(
            singleton<settingswindow>().from_map("lecostumword").value<QString> ());

    volumechange(singleton<snpsettings>().map["volumeslidervalue"].value<int>());
}
void sound_handler::volumechange(int i) {
    startupsoundoutput->setVolume(double(i) / 10);
    chatwindowopensoundoutput->setVolume(double(i) / 10);
    normalmsgsoundoutput->setVolume(double(i) / 10);
    chatwindowopensoundoutput->setVolume(double(i) / 10);
    buddyarrivedsoundoutput->setVolume(double(i) / 10);
    buddyleftsoundoutput->setVolume(double(i) / 10);
    costumwordsoundsoundoutput->setVolume(double(i) / 10);
    singleton<snpsettings>().map["volumeslidervalue"].setValue<int> (i);
}

void sound_handler::play_startupsound(){    
    if (!singleton<settingswindow>().from_map("cbstartup").value<bool> ())
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    startupsound->stop();
    startupsound->play();   
}

void sound_handler::play_normalmsgsound(const QString user){
    if (!singleton<settingswindow>().from_map("cbplaynormalchatmessage").value<bool> ())
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
    if (!singleton<settingswindow>().from_map("cbplaybuddychatmessage").value<bool> ())
        return;
    try{
        verify_if_played(user);
    } catch(dont_play_sound_exception){
        return;
    }
    buddymsgsound->stop();
    buddymsgsound->play();
}
void sound_handler::play_chatwindowopensound(){
    if (!singleton<settingswindow>().from_map("cbplaybuddychatwindowopened").value<bool> ())
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    chatwindowopensound->stop();
    chatwindowopensound->play();
}
void sound_handler::play_buddyleftsound(){
    if (!singleton<settingswindow>().from_map("cbbuddyleaves").value<bool> ())
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    buddyleftsound->play();
    buddyleftsound->play();
}
void sound_handler::play_buddyarrivedsound(){
    if (!singleton<settingswindow>().from_map("cbbuddyarrives").value<bool> ())
        return;
    try{
        verify_if_played();
    } catch(dont_play_sound_exception){
        return;
    }
    buddyarrivedsound->stop();
    buddyarrivedsound->play();
}
void sound_handler::play_highlightningsound(const QString user, QWidget *w){
    if (!singleton<settingswindow>().from_map("cbhighlightning").value<bool> ())
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
    if (!singleton<settingswindow>().from_map("chbhostsound").toBool())
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
    if (!singleton<settingswindow>().from_map("cbcostumword").value<bool> ())
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
    if(singleton<settingswindow>().from_map("cbdontplaysound").toBool())
        throw dont_play_sound_exception();
    if(user==QString())
        return;
    if (net->mutedusers.contains(user, Qt::CaseInsensitive))
        throw dont_play_sound_exception();
}
sound_handler::~sound_handler(){
}
