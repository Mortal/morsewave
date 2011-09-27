#include "morsegenerator.h"
#include <sndfile.hh>
#include <QtCore/qmath.h>
#include <cstring>
#include <QErrorMessage>
#include <sstream>

enum morse {
    END, DIT, DAH
};

enum breaktype {
    NONE, LETTER, WORD, SENTENCE
};

const morse * code(const QChar c) {
    switch (c.toLatin1()) {
    case 'A': return (const morse[]) { DIT, DAH, END };
    case 'B': return (const morse[]) { DAH, DIT, DIT, DIT, END };
    case 'C': return (const morse[]) { DAH, DIT, DAH, DIT, END };
    case 'D': return (const morse[]) { DAH, DIT, DIT, END };
    case 'E': return (const morse[]) { DIT, END };
    case 'F': return (const morse[]) { DIT, DIT, DAH, DIT, END };
    case 'G': return (const morse[]) { DAH, DAH, DIT, END };
    case 'H': return (const morse[]) { DIT, DIT, DIT, DIT, END };
    case 'I': return (const morse[]) { DIT, DIT, END };
    case 'J': return (const morse[]) { DIT, DAH, DAH, DAH, END };
    case 'K': return (const morse[]) { DAH, DIT, DAH, END };
    case 'L': return (const morse[]) { DIT, DAH, DIT, DIT, END };
    case 'M': return (const morse[]) { DAH, DAH, END };
    case 'N': return (const morse[]) { DAH, DIT, END };
    case 'O': return (const morse[]) { DAH, DAH, DAH, END };
    case 'P': return (const morse[]) { DIT, DAH, DAH, DIT, END };
    case 'Q': return (const morse[]) { DAH, DAH, DIT, DAH, END };
    case 'R': return (const morse[]) { DIT, DAH, DIT, END };
    case 'S': return (const morse[]) { DIT, DIT, DIT, END };
    case 'T': return (const morse[]) { DAH, END };
    case 'U': return (const morse[]) { DIT, DIT, DAH, END };
    case 'V': return (const morse[]) { DIT, DIT, DIT, DAH, END };
    case 'W': return (const morse[]) { DIT, DAH, DAH, END };
    case 'X': return (const morse[]) { DAH, DIT, DIT, DAH, END };
    case 'Y': return (const morse[]) { DAH, DIT, DAH, DAH, END };
    case 'Z': return (const morse[]) { DAH, DAH, DIT, DIT, END };
    case '0': return (const morse[]) { DAH, DAH, DAH, DAH, DAH, END };
    case '1': return (const morse[]) { DIT, DAH, DAH, DAH, DAH, END };
    case '2': return (const morse[]) { DIT, DIT, DAH, DAH, DAH, END };
    case '3': return (const morse[]) { DIT, DIT, DIT, DAH, DAH, END };
    case '4': return (const morse[]) { DIT, DIT, DIT, DIT, DAH, END };
    case '5': return (const morse[]) { DIT, DIT, DIT, DIT, DIT, END };
    case '6': return (const morse[]) { DAH, DIT, DIT, DIT, DIT, END };
    case '7': return (const morse[]) { DAH, DAH, DIT, DIT, DIT, END };
    case '8': return (const morse[]) { DAH, DAH, DAH, DIT, DIT, END };
    case '9': return (const morse[]) { DAH, DAH, DAH, DAH, DIT, END };
    case '.': return (const morse[]) { DIT, DAH, DIT, DAH, DIT, DAH, END };
    case ',': return (const morse[]) { DAH, DAH, DIT, DIT, DAH, DAH, END };
    case '?': return (const morse[]) { DIT, DIT, DAH, DAH, DIT, DIT, END };
    case '\'':return (const morse[]) { DIT, DAH, DAH, DAH, DAH, DIT, END };
    case '!': return (const morse[]) { DAH, DIT, DAH, DIT, DAH, DAH, END };
    case '/': return (const morse[]) { DAH, DIT, DIT, DAH, DIT, END };
    case '(': return (const morse[]) { DAH, DIT, DAH, DAH, DIT, END };
    case ')': return (const morse[]) { DAH, DIT, DAH, DAH, DIT, DAH, END };
    case '&': return (const morse[]) { DIT, DAH, DIT, DIT, DIT, END };
    case ':': return (const morse[]) { DAH, DAH, DAH, DIT, DIT, DIT, END };
    case ';': return (const morse[]) { DAH, DIT, DAH, DIT, DAH, DIT, END };
    case '=': return (const morse[]) { DAH, DIT, DIT, DIT, DAH, END };
    case '+': return (const morse[]) { DIT, DAH, DIT, DAH, DIT, END };
    case '-': return (const morse[]) { DAH, DIT, DIT, DIT, DIT, DAH, END };
    case '_': return (const morse[]) { DIT, DIT, DAH, DAH, DIT, DAH, END };
    case '"': return (const morse[]) { DIT, DAH, DIT, DIT, DAH, DIT, END };
    case '$': return (const morse[]) { DIT, DIT, DIT, DAH, DIT, DIT, DAH, END };
    case '@': return (const morse[]) { DIT, DAH, DAH, DIT, DAH, DIT, END };
    case '\xC6': return (const morse[]) { DIT, DAH, DIT, DAH, END }; // ae ligature
    case '\xD8': return (const morse[]) { DAH, DAH, DAH, DIT, END }; // o slash
    case '\xC5': return (const morse[]) { DIT, DAH, DAH, DIT, DAH, END }; // a ring
    }
    return (const morse[]) { END };
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
            const morse * atoms = code(*i);
            if (atoms[0] == END) {
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
                while (*atoms != END) {
#ifndef QT_NO_DEBUG
                    debug << "Insert atom " << *atoms << std::endl;
#endif
                    if (*atoms == DIT) {
                        snd.writef(signal, ditlength);
                        snd.writef(silence, atomlength-ditlength);
                    } else if (*atoms == DAH) {
                        snd.writef(signal, dahlength);
                        snd.writef(silence, atomlength-dahlength);
                    }
                    ++atoms;
                }
            }
        }
    }
    snd.writef(silence, wordpause);

    if (!debug.str().empty()) {
        QErrorMessage::qtHandler()->showMessage(debug.str().c_str());
    }
}
