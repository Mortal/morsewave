#include "morsegenerator.h"
#include <sndfile.hh>
#include <QtCore/qmath.h>
#include <cstring>
#include <QErrorMessage>
#include <sstream>
#include <iostream>

enum breaktype {
    NONE, LETTER, WORD, SENTENCE
};

std::string code(const QChar c) {
    switch (c.toLatin1()) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case '0': return "-----";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case '.': return ".-.-.-";
    case ',': return "--..--";
    case '?': return "..--..";
    case '\'':return ".----.";
    case '!': return "-.-.--";
    case '/': return "-..-.";
    case '(': return "-.--.";
    case ')': return "-.--.-";
    case '&': return ".-...";
    case ':': return "---...";
    case ';': return "-.-.-.";
    case '=': return "-...-";
    case '+': return ".-.-.";
    case '-': return "-....-";
    case '_': return "..--.-";
    case '"': return ".-..-.";
    case '$': return "...-..-";
    case '@': return ".--.-.";
    case '\xC6': return ".-.-"; // ae ligature
    case '\xD8': return "---."; // o slash
    case '\xC5': return ".--.-"; // a ring
    }
    return 0;
}

MorseGenerator::MorseGenerator(QString input, std::string filename) :
input(input), filename(filename) {
    // Empty ctor.
}
void reccpy(void * ptr, int initial, int length) {
    while (initial < length) {
        int amount = std::min(length-initial, initial);
        memcpy((quint8 *) ptr+initial, ptr, amount);
        initial += amount;
    }
}
void MorseGenerator::generate() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int channels = 1;
    const int samplerate = 44100;
    const int frequency = 440; // signal frequency
    const float amplitude = 0.7;
    const int letterpause = samplerate*3/2; // 1.5 seconds
    const int wordpause = samplerate*3; // 3 seconds
    const int sentencepause = samplerate*6; // 6 seconds
    const int atomfrequency = 3; // atoms (dit/dah) per second
    const int atomlength = samplerate/atomfrequency;
    const int ditlength = atomlength/3;
    const int dahlength = 2*atomlength/3;
    SndfileHandle snd(filename, SFM_WRITE, format, channels, samplerate);
    if (snd.error() != SF_ERR_NO_ERROR) {
        QErrorMessage::qtHandler()->showMessage(("When trying to create sound "+filename+": "+snd.strError()).c_str());
        return;
    }

    const int wavelength = samplerate/frequency;

    float signal[dahlength];
    qreal pi = (qreal) 2.0*qAcos(0);
    for (int i = 0; i < wavelength; ++i) {
        signal[i] = amplitude * qSin((qreal) 2.0 * i * pi / (qreal) wavelength);
    }
    reccpy(&signal[0], wavelength*sizeof(int), dahlength*sizeof(int));

    float silence[sentencepause];
    silence[0] = 0.0;
    reccpy(&silence[0], sizeof(float), sentencepause*sizeof(float));

    std::stringstream debug;

#ifndef QT_NO_DEBUG
    debug << "Dit length: " << ditlength << " Dah length: " << dahlength << " atom length: " << atomlength << std::endl;
#endif
    breaktype nextbreak = NONE;
    for (QString::iterator i = input.begin(); i != input.end(); ++i) {
        if (*i == ' ') {
            if (nextbreak < WORD)
                nextbreak = WORD;
        } else if (*i == '.') {
            if (nextbreak < SENTENCE)
                nextbreak = SENTENCE;
        } else {
            std::string atoms = code(*i);
            if (atoms.empty()) {
                debug << "Unknown character " << i->unicode() << std::endl;
            } else {
#ifndef QT_NO_DEBUG
                debug << "Insert " << nextbreak << std::endl;
#endif
                switch (nextbreak) {
                case NONE:
                    break;
                case LETTER:
                    snd.writef(silence, letterpause);
                    break;
                case WORD:
                    snd.writef(silence, wordpause);
                    break;
                case SENTENCE:
                    snd.writef(silence, sentencepause);
                    break;
                }
                nextbreak = LETTER;
                for (std::string::iterator i = atoms.begin(); i != atoms.end(); ++i) {
                    if (*i == '.') {
                        snd.writef(signal, ditlength);
                        snd.writef(silence, atomlength-ditlength);
                    } else if (*i == '-') {
                        snd.writef(signal, dahlength);
                        snd.writef(silence, atomlength-dahlength);
                    }
                }
            }
        }
    }
    snd.writef(silence, wordpause);
}
