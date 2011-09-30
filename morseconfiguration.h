#ifndef MORSECONFIGURATION_H
#define MORSECONFIGURATION_H

struct MorseConfiguration {
    int dit;
    int ditpause;
    int dah;
    int dahpause;
    int letterseparator;
    int wordseparator;
    int sentenceseparator;
    int frequency;
    MorseConfiguration(int dit, int ditpause, int dah, int dahpause, int letterseparator, int wordseparator, int sentenceseparator, int frequency) :
    dit(dit),
    ditpause(ditpause),
    dah(dah),
    dahpause(dahpause),
    letterseparator(letterseparator),
    wordseparator(wordseparator),
    sentenceseparator(sentenceseparator),
    frequency(frequency) {
        // empty ctor
    }
};

#endif // MORSECONFIGURATION_H
