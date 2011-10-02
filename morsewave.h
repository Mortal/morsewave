#ifndef MORSEWAVE_H
#define MORSEWAVE_H

#include <QtGui>
#include "morseconfigurationmodel.h"

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

    void displayMessage(const QString & err);

    QString filename();
    QString getInput();
    QString getDir();

    void readSettings();
    void writeSettings();

    void closeEvent(QCloseEvent *);

    MorseConfigurationModel * config;

    QGridLayout * layout;
    QLabel * lblInput;
    QLabel * lblDir;
    QTextEdit * input;
    QLineEdit * dir;
    QPushButton * dirbrowse;
    QPushButton * generate;
    QTableView * generationsettings;
};

#endif // MORSEWAVE_H
