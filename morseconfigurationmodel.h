#ifndef MORSECONFIGURATIONMODEL_H
#define MORSECONFIGURATIONMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QSettings>
#include "morseconfiguration.h"

class MorseConfigurationModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MorseConfigurationModel(QObject *parent = 0);
    virtual ~MorseConfigurationModel() {}
    virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex & index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    void readSettings(QSettings & settings);
    void writeSettings(QSettings & settings);
    MorseConfiguration getAll();

signals:

public slots:

private:
    int dit;
    int ditpause;
    int dah;
    int dahpause;
    int letterseparator;
    int wordseparator;
    int sentenceseparator;
    int frequency;
    QStringList keys;

};

#endif // MORSECONFIGURATIONMODEL_H
