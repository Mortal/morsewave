#ifndef MORSEPLAYER_H
#define MORSEPLAYER_H

#include <QObject>
#include <QString>
#include <QTemporaryFile>
#include <Phonon/MediaObject>

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
