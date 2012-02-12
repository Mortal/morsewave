#include "morseplayer.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

MorsePlayer::MorsePlayer(QObject *parent) :
    QObject(parent),
    player(0)
{
    QCoreApplication::setApplicationName("MorseWave");
}

MorsePlayer::~MorsePlayer() {
    if (player != 0) {
        player->stop();
        delete player;
        player = 0;
    }
    stopped();
}

QString MorsePlayer::filename() {
    return QDir::tempPath()+"/morsewave.wav";
}

void MorsePlayer::stopped() {
    QFile(filename()).remove();
}

void MorsePlayer::play() {
    if (player == 0) {
        player = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(filename()));
        connect(player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    }
    if (player != 0)
        player->play();
}

void MorsePlayer::stateChanged(Phonon::State to, Phonon::State /*from*/) {
    if (to == Phonon::ErrorState)
        qDebug() << "Phonon error:" << player->errorString();
    if (to != Phonon::PlayingState && to != Phonon::LoadingState && to != Phonon::BufferingState)
        stopped();
}

void MorsePlayer::stop() {
    if (player != 0)
        player->stop();
}
