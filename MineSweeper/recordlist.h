#ifndef RECORDLIST_H
#define RECORDLIST_H

#include "gamemode_def.h"
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QDateTime>
#include <QSettings>
#include <QPushButton>

class RecordList: public QWidget
{
    Q_OBJECT
public:
    RecordList();
    void Read();
    void Record(GameMode game_mode, int record_time);
    void Reset();
public slots:
    void Clear();
private:
    QLabel record_label_[9];
    QSettings* record_;
    QPushButton record_clear_;
};

#endif // RECORDLIST_H
