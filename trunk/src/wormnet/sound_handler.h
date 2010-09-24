#ifndef SOUND_HANDLER_H
#define SOUND_HANDLER_H

#include <QObject>
#include<stdexcept>
#include"global_macros.h"
namespace Phonon{
    class MediaSource;
    class MediaObject;
    class AudioOutput;
}
class sound_handler : public QObject
{
        Q_OBJECT
    DECLARE_SINGLETON(sound_handler);
public:
    void init();                //used when settingswindowmap changes
public slots:
    void play_startupsound();
    void play_normalmsgsound(const QString user);
    void play_buddymsgsound(const QString user);
    void play_chatwindowopensound();
    void play_buddyleftsound();
    void play_buddyarrivedsound();
    void play_highlightningsound(const QString user, QWidget *w);
    void play_buddyhostedsound();
    void play_costumwordsound(const QString user, QWidget *w);
private:
//    Phonon::MediaObject *SoundObject;

    Phonon::MediaObject *startupsound;
    Phonon::MediaObject	*normalmsgsound;
    Phonon::MediaObject	*buddymsgsound;
    Phonon::MediaObject *chatwindowopensound;
    Phonon::MediaObject	*buddyleftsound;
    Phonon::MediaObject	*buddyarrivedsound;
    Phonon::MediaObject	*normalchatsound;
    Phonon::MediaObject	*highlightningsound;
    Phonon::MediaObject *buddyhostedsound;
    Phonon::MediaObject *costumwordsound;

//    Phonon::AudioOutput *SoundOutput;

    Phonon::AudioOutput *startupsoundoutput;
    Phonon::AudioOutput *chatwindowopensoundoutput;
    Phonon::AudioOutput *normalmsgsoundoutput;
    Phonon::AudioOutput *buddymsgsoundoutput;
    Phonon::AudioOutput *highlightningoutput;
    Phonon::AudioOutput	*normalchatsoundoutput;
    Phonon::AudioOutput	*buddyleftsoundoutput;
    Phonon::AudioOutput	*buddyarrivedsoundoutput;
    Phonon::AudioOutput *buddyhostedsoundoutput;
    Phonon::AudioOutput *costumwordsoundsoundoutput;

    class dont_play_sound_exception: public std::exception{};

    void verify_if_played(const QString user=QString()) throw(dont_play_sound_exception);

signals:

public slots:
    void volumechange(int i);


};

#endif // SOUND_HANDLER_H
