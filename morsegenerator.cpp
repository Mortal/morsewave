#include "morsegenerator.h"
#include <QtCore/qmath.h>
#include <QTextStream>

enum breaktype {
    NONE, LETTER, WORD, SENTENCE
};

QString code(const QChar c) {
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

MorseGenerator::MorseGenerator(const QString & input, const QString & filename, const MorseConfiguration & config) :
input(input), filename(filename), config(config) {
    // Empty ctor.
}
void reccpy(void * ptr, int initial, int length) {
    while (initial < length) {
        int amount = std::min(length-initial, initial);
        memcpy((quint8 *) ptr+initial, ptr, amount);
        initial += amount;
    }
}
void MorseGenerator::snderrorcheck() {
    if (snd->error() != SF_ERR_NO_ERROR) {
        throw sndfileerror(snd->strError());
        snd.reset(0);
        return;
    }
}
void MorseGenerator::generate() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int channels = 1;
    const int samplerate = 44100;
    const int frequency = config.frequency; // signal frequency
    const float amplitude = 0.7;
    const int letterpause = config.letterseparator*samplerate/1000; // 1.5 seconds
    const int wordpause = config.wordseparator*samplerate/1000; // 3 seconds
    const int sentencepause = config.sentenceseparator*samplerate/1000; // 6 seconds
    const int ditlength = config.dit*samplerate/1000;
    const int dahlength = config.dah*samplerate/1000;
    const int ditpause = config.ditpause*samplerate/1000;
    const int dahpause = config.dahpause*samplerate/1000;
    const int signallength = std::max(ditlength,dahlength);
    const int silencelength = std::max(std::max(std::max(letterpause, wordpause), sentencepause), std::max(ditpause, dahpause));
    snd.reset(new SndfileHandle(filename.toStdString(), SFM_WRITE, format, channels, samplerate));
    snderrorcheck();

    const int wavelength = samplerate/frequency;

    float signal[signallength];
    qreal pi = (qreal) 2.0*qAcos(0);
    for (int i = 0; i < wavelength; ++i) {
        signal[i] = amplitude * qSin((qreal) 2.0 * i * pi / (qreal) wavelength);
    }
    reccpy(&signal[0], wavelength*sizeof(int), signallength*sizeof(int));

    float silence[silencelength];
    silence[0] = 0.0;
    reccpy(&silence[0], sizeof(float), silencelength*sizeof(float));

    QTextStream debug;

    breaktype nextbreak = NONE;
    for (QString::iterator i = input.begin(); i != input.end(); ++i) {
        if (*i == ' ') {
            if (nextbreak < WORD)
                nextbreak = WORD;
        } else if (*i == '.') {
            if (nextbreak < SENTENCE)
                nextbreak = SENTENCE;
        } else {
            QString atoms = code(*i);
            if (atoms.isEmpty()) {
                debug << "Unknown character " << i->unicode() << endl;
            } else {
#ifndef QT_NO_DEBUG
                debug << "Insert " << nextbreak << endl;
#endif
                switch (nextbreak) {
                case NONE:
                    break;
                case LETTER:
                    snd->writef(silence, letterpause);
                    break;
                case WORD:
                    snd->writef(silence, wordpause);
                    break;
                case SENTENCE:
                    snd->writef(silence, sentencepause);
                    break;
                }
                snderrorcheck();
                nextbreak = LETTER;
                for (QChar * i = atoms.begin(); i != atoms.end(); ++i) {
                    if (*i == '.') {
                        snd->writef(signal, ditlength);
                        snd->writef(silence, ditpause);
                    } else if (*i == '-') {
                        snd->writef(signal, dahlength);
                        snd->writef(silence, dahpause);
                    }
                    snderrorcheck();
                }
            }
        }
    }
    snd->writef(silence, wordpause);
    snderrorcheck();
    snd.reset(0);
}
