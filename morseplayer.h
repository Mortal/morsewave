// vim:set ts=8 sw=4 sts=4 et:
#ifndef MORSEPLAYER_H
#define MORSEPLAYER_H

#include <QObject>
#include <QString>
#include <QTemporaryFile>
#include <phonon/MediaObject>

class MorsePlayer : public QObject
{
    Q_OBJECT
public:
    explicit MorsePlayer(QObject *parent = 0);
    virtual ~MorsePlayer();
    QString filename();
    void play();
    void stop();
    void stopped();

signals:

public slots:
    void stateChanged(Phonon::State to, Phonon::State from);

private:
    QString _filename;
    Phonon::MediaObject * player;

};

#endif // MORSEPLAYER_H
