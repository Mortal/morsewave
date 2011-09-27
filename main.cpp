#include <QtGui>
#include "morsewave.h"
#include "morsegenerator.h"

MorseWave::MorseWave(QApplication & app) {
    makeLayout(app);
}

void MorseWave::makeLayout(QApplication & app) {
    layout = new QGridLayout();

    lblInput = new QLabel(tr("Text"));
    lblDir = new QLabel(tr("Output folder"));
    input = new QTextEdit();
    input->setAcceptRichText(false);
    dir = new QLineEdit();
    dirbrowse = new QPushButton();
    dirbrowse->setText(tr("Browse..."));
    QObject::connect(dirbrowse, SIGNAL(clicked()), this, SLOT(promptDir()));
    generate = new QPushButton();
    generate->setText(tr("Generate"));
    QObject::connect(generate, SIGNAL(clicked()), this, SLOT(gen()));

    layout->addWidget(lblInput, 0, 0);
    layout->addWidget(input, 0, 1, 1, 2);
    layout->addWidget(lblDir, 1, 0);
    layout->addWidget(dir, 1, 1);
    layout->addWidget(dirbrowse, 1, 2);

    layout->addWidget(generate, 2, 0, 1, 3, Qt::AlignHCenter);

    setLayout(layout);
}

void MorseWave::promptDir() {
    dir->setText(QFileDialog::getExistingDirectory());
}

/**
 * Return user-specified input text, trimmed, in uppercase.
 */
QString MorseWave::getInput() {
    return input->toPlainText().trimmed().replace(QRegExp("   *"), " ").toUpper();
}

/**
 * Return user-specified directory, trimmed, using forward slashes, with final forward slash.
 */
QString MorseWave::getDir() {
    QString dirname = dir->text().trimmed();
    if (!dirname.contains('/') && dirname.contains('\\'))
        dirname = dirname.replace('\\', '/');
    if (!dirname.endsWith('/'))
        dirname += '/';
    return dirname;
}

QString MorseWave::filename() {
    QString dirname = getDir();
    QString prefix = getInput().left(12).replace(QRegExp("[^\\w\\d]+"), " ").trimmed();
    const QString suffix = ".wav";
    QFile target(dirname+prefix+suffix);
    for (int i = 2; target.exists(); ++i) {
        target.setFileName(dirname+prefix+"-"+QString::number(i)+suffix);
    }
    return target.fileName();
}

void MorseWave::gen() {
    MorseGenerator generator(getInput(), QDir::toNativeSeparators(filename()).toStdString());
    generator.generate();
}

int main(int argv, char **args)
{
    QApplication app(argv, args);

    MorseWave mw(app);

    mw.show();

    return app.exec();
}
