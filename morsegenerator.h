#ifndef MORSEGENERATOR_H
#define MORSEGENERATOR_H

#include <string>
#include <QString>

class MorseGenerator {
public:
    MorseGenerator(QString input, std::string filename);
    void generate();

private:
    QString input;
    std::string filename;
};

#endif // MORSEGENERATOR_H
