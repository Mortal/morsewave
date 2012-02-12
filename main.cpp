// vim:set ts=8 sw=4 sts=4 et:
#include <QtGui>
#include "morsewave.h"
#include "morsegenerator.h"
#include "morsewavesettings.h"
#include <QErrorMessage>

MorseWave::MorseWave(QApplication & app) {
    config = new MorseConfigurationModel();
    makeLayout(app);
    readSettings();
}

void MorseWave::closeEvent(QCloseEvent */*event*/) {
    writeSettings();
}

void MorseWave::makeLayout(QApplication & /*app*/) {
    layout = new QGridLayout();

    lblInput = new QLabel(tr("Text"));
    lblDir = new QLabel(tr("Output folder"));
    input = new QTextEdit();
    input->setAcceptRichText(false);
    input->setTabChangesFocus(true);
    dir = new QLineEdit();
    dirbrowse = new QPushButton();
    dirbrowse->setText(tr("Browse..."));
    QObject::connect(dirbrowse, SIGNAL(clicked()), this, SLOT(promptDir()));
    buttons = new QWidget();
    buttonlayout = new QHBoxLayout();
    generate = new QPushButton();
    generate->setText(tr("Generate"));
    QObject::connect(generate, SIGNAL(clicked()), this, SLOT(gen()));
    playbutton = new QPushButton();
    playbutton->setText(tr("Play"));
    QObject::connect(playbutton, SIGNAL(clicked()), this, SLOT(play()));

    generationsettings = new QTableView();
    generationsettings->setModel(config);
    generationsettings->resize(100, 100);

    int y = 0;

    layout->addWidget(lblInput, y, 0);
    layout->addWidget(input, y, 1, 1, 2);
    ++y;
    layout->addWidget(lblDir, y, 0);
    layout->addWidget(dir, y, 1);
    layout->addWidget(dirbrowse, y, 2);
    ++y;
    layout->addWidget(generationsettings, y, 1, 1, 2);
    ++y;
    layout->addWidget(buttons, y, 0, 1, 3, Qt::AlignHCenter);
    buttons->setLayout(buttonlayout);
    buttonlayout->addWidget(generate);
    buttonlayout->addWidget(playbutton);

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
    if (dirname == "")
        return dirname;

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

void MorseWave::displayMessage(const QString & err) {
    QErrorMessage::qtHandler()->showMessage(err);
}

void MorseWave::gen() {
    if (getDir() == "") {
        displayMessage("Choose the destination directory");
        return;
    }
    try {
        QString file = QDir::toNativeSeparators(filename());
        MorseGenerator generator(getInput(), file, config->getAll());
        generator.generate();
        displayMessage("Created sound file "+file);
    } catch (morsegeneratorexception ex) {
        displayMessage(ex.what());
    }
}

void MorseWave::play() {
    player.reset(new MorsePlayer());
    {
        MorseGenerator generator(getInput(), player->filename(), config->getAll());
        generator.generate();
    }
    player->play();
}

void MorseWave::readSettings() {
    MorseWaveSettings settings;
    settings.beginGroup("window");
    resize(settings.value("size", QSize(350, 300)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    settings.beginGroup("morse");
    config->readSettings(settings);
    settings.endGroup();
    dir->setText(settings.value("directory", "").toString());
}

void MorseWave::writeSettings() {
    MorseWaveSettings settings;
    settings.beginGroup("window");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    settings.beginGroup("morse");
    config->writeSettings(settings);
    settings.endGroup();
    settings.setValue("directory", dir->text());
}

int main(int argv, char **args) {
    QApplication app(argv, args);

    MorseWave mw(app);

    mw.show();

    return app.exec();
}
