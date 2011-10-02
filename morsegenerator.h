#ifndef MORSEGENERATOR_H
#define MORSEGENERATOR_H

#include <QString>
#include <QScopedPointer>
#include <sndfile.hh>
#include "morseconfiguration.h"

class MorseGenerator {
public:
    MorseGenerator(const QString & input, const QString & filename, const MorseConfiguration & config);
    void generate();

private:
    QString input;
    QString filename;
    QScopedPointer<SndfileHandle> snd;
    void snderrorcheck();
    MorseConfiguration config;
    static QString code(const QChar c);
};

class morsegeneratorexception {
public:
    morsegeneratorexception(const QString & what) : _what(what) {}
    QString what() {
        return _what;
    }
private:
    QString _what;
};

class sndfileerror : public morsegeneratorexception {
public:
    sndfileerror(const QString & what) : morsegeneratorexception(what) {}
};

class morseinputerror : public morsegeneratorexception {
    morseinputerror(const QString & what) : morsegeneratorexception(what) {}
};

#endif // MORSEGENERATOR_H
