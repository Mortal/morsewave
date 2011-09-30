#ifndef MORSEWAVESETTINGS_H
#define MORSEWAVESETTINGS_H

#include <QSettings>

class MorseWaveSettings : public QSettings {
Q_OBJECT
public:
    MorseWaveSettings();
private:
    QSettings settings;
};

#endif // MORSEWAVESETTINGS_H
