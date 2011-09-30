#ifndef MORSEGENERATOR_H
#define MORSEGENERATOR_H

#include <string>
#include <QString>
#include <stdexcept>
#include <memory>

class MorseGenerator {
public:
    MorseGenerator(QString input, std::string filename);
    void generate();

private:
    QString input;
    std::string filename;
    std::auto_ptr<class SndfileHandle> snd;
    void snderrorcheck();
};

class morsegeneratorexception : public std::exception {
public:
    morsegeneratorexception(const std::string & what) : _what(what.c_str()) {}
    morsegeneratorexception(const char * what) : _what(what) {}
    const char * what() {
        return _what;
    }
private:
    const char * _what;
};

class sndfileerror : public morsegeneratorexception {
public:
    sndfileerror(const std::string & what) : morsegeneratorexception(what.c_str()) {}
    sndfileerror(const char * what) : morsegeneratorexception(what) {}
};

class morseinputerror : public morsegeneratorexception {
    morseinputerror(const std::string & what) : morsegeneratorexception(what.c_str()) {}
    morseinputerror(const char * what) : morsegeneratorexception(what) {}
};

#endif // MORSEGENERATOR_H
