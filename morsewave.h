#ifndef MORSEWAVE_H
#define MORSEWAVE_H

#include <QtGui>

class MorseWave : public QWidget {
    Q_OBJECT

public:
    MorseWave(QApplication &);

public slots:
    void promptDir();
    void gen();

private:
    void makeLayout(QApplication &);
    QWidget * makeLayoutInput();

    QString filename();
    QString getInput();
    QString getDir();

    QGridLayout * layout;
    QLabel * lblInput;
    QLabel * lblDir;
    QTextEdit * input;
    QLineEdit * dir;
    QPushButton * dirbrowse;
    QPushButton * generate;
};

#endif // MORSEWAVE_H
