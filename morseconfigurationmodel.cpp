#include "morseconfigurationmodel.h"

MorseConfigurationModel::MorseConfigurationModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    keys << "dit" << "ditpause" << "dah" << "dahpause"
         << "letterseparator" << "wordseparator"
         << "sentenceseparator" << "frequency";
    dit = dahpause = 1000/9;
    ditpause = dah = 2000/9;
    letterseparator = 1500;
    wordseparator = 3000;
    sentenceseparator = 6000;
    frequency = 440;
}

void MorseConfigurationModel::readSettings(QSettings & settings) {
    dit = settings.value("dit", dit).toInt();
    ditpause = settings.value("ditpause", ditpause).toInt();
    dah = settings.value("dah", dah).toInt();
    dahpause = settings.value("dahpause", dahpause).toInt();
    letterseparator = settings.value("letterseparator", letterseparator).toInt();
    wordseparator = settings.value("wordseparator", wordseparator).toInt();
    sentenceseparator = settings.value("sentenceseparator", sentenceseparator).toInt();
    frequency = settings.value("frequency", frequency).toInt();
}

void MorseConfigurationModel::writeSettings(QSettings & settings) {
    settings.setValue("dit", dit);
    settings.setValue("ditpause", ditpause);
    settings.setValue("dah", dah);
    settings.setValue("dahpause", dahpause);
    settings.setValue("letterseparator", letterseparator);
    settings.setValue("wordseparator", wordseparator);
    settings.setValue("sentenceseparator", sentenceseparator);
    settings.setValue("frequency", frequency);
}

QModelIndex MorseConfigurationModel::index(int row, int column, const QModelIndex & parent) const {
    if (parent.isValid()) return QModelIndex();
    if (row >= 0 && row < rowCount() && column == 0)
        return createIndex(row, column);
    return QModelIndex();
}

QModelIndex MorseConfigurationModel::parent(const QModelIndex & index) const {
    return QModelIndex();
}

int MorseConfigurationModel::rowCount(const QModelIndex & parent) const {
    return parent.isValid() ? 0 : 8;
}

int MorseConfigurationModel::columnCount(const QModelIndex & parent) const {
    return parent.isValid() ? 0 : 1;
}

QVariant MorseConfigurationModel::data(const QModelIndex & index, int role) const {
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    if (!index.isValid()) return QVariant();
    if (index.column() != 0) return QVariant();
    if (role == Qt::DisplayRole) return data(index, Qt::EditRole).toString() + ((index.row() == 7) ? " Hz" : " ms");
    switch (index.row()) {
    case 0:
        return QString::number(dit);
    case 1:
        return QString::number(ditpause);
    case 2:
        return QString::number(dah);
    case 3:
        return QString::number(dahpause);
    case 4:
        return QString::number(letterseparator);
    case 5:
        return QString::number(wordseparator);
    case 6:
        return QString::number(sentenceseparator);
    case 7:
        return QString::number(frequency);
    }
    return QVariant();
}

bool MorseConfigurationModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (!index.isValid() || role != Qt::EditRole) return false;
    int row = index.row();
    if (index.column() != 0) return false;
    switch (row) {
    case 0:
        dit = value.toString().toInt();
        return true;
    case 1:
        ditpause = value.toString().toInt();
        return true;
    case 2:
        dah = value.toString().toInt();
        return true;
    case 3:
        dahpause = value.toString().toInt();
        return true;
    case 4:
        letterseparator = value.toString().toInt();
        return true;
    case 5:
        wordseparator = value.toString().toInt();
        return true;
    case 6:
        sentenceseparator = value.toString().toInt();
        return true;
    case 7:
        frequency = value.toString().toInt();
        return true;
    }
    return false;
}

Qt::ItemFlags MorseConfigurationModel::flags(const QModelIndex & index) const {
    if (!index.isValid()) return 0;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant MorseConfigurationModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        if (role == Qt::SizeHintRole)
            return 0;
        return QVariant();
    }
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case 0:
        return tr("dit");
    case 1:
        return tr("ditpause");
    case 2:
        return tr("dah");
    case 3:
        return tr("dahpause");
    case 4:
        return tr("letterseparator");
    case 5:
        return tr("wordseparator");
    case 6:
        return tr("sentenceseparator");
    case 7:
        return tr("frequency");
    }
    return QVariant();
}

MorseConfiguration MorseConfigurationModel::getAll() {
    return MorseConfiguration(dit, ditpause, dah, dahpause, letterseparator, wordseparator, sentenceseparator, frequency);
}
