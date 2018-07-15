#include "recordlist.h"

RecordList::RecordList()
{
    this->setWindowTitle("Records");
    this->setFixedSize(360,180);
    record_label_[0].setText("Easy:");
    record_label_[3].setText("Normal:");
    record_label_[6].setText("Hard:");
    for(int i = 0; i < 9;i ++){
        record_label_[i].setParent(this);
        record_label_[i].setFixedSize(i%3==1?200:60,25);
        record_label_[i].move(i%3==2?300:30+(i%3)*60, 30+(i/3)*30);
        record_label_[i].show();
    }

    record_ = new QSettings("records.ini", QSettings::IniFormat);
    record_->beginGroup("RECORDS");
    record_label_[1].setText(record_->value("EASY_RECORD_DATE").toString());
    record_label_[2].setText(record_->value("EASY_RECORD_TIME").toString());
    record_label_[4].setText(record_->value("NORMAL_RECORD_DATE").toString());
    record_label_[5].setText(record_->value("NORMAL_RECORD_TIME").toString());
    record_label_[7].setText(record_->value("HARD_RECORD_DATE").toString());
    record_label_[8].setText(record_->value("HARD_RECORD_TIME").toString());
    record_->endGroup();

    record_clear_.setParent(this);
    record_clear_.setFixedSize(50,25);
    record_clear_.move(155,135);
    record_clear_.setText("Clear");
    record_clear_.show();
    QObject::connect(&this->record_clear_, SIGNAL(released()), this, SLOT(Clear()));
}

void RecordList::Read(){
    record_->beginGroup("RECORDS");
    record_label_[1].setText(record_->value("EASY_RECORD_DATE").toString());
    record_label_[2].setText(record_->value("EASY_RECORD_TIME").toString());
    record_label_[4].setText(record_->value("NORMAL_RECORD_DATE").toString());
    record_label_[5].setText(record_->value("NORMAL_RECORD_TIME").toString());
    record_label_[7].setText(record_->value("HARD_RECORD_DATE").toString());
    record_label_[8].setText(record_->value("HARD_RECORD_TIME").toString());
    record_->endGroup();
}

void RecordList::Reset(){
    record_->clear();
    record_->beginGroup("RECORDS");
    record_->setValue("EASY_RECORD_DATE","--");
    record_->setValue("EASY_RECORD_TIME","999");
    record_->setValue("NORMAL_RECORD_DATE","--");
    record_->setValue("NORMAL_RECORD_TIME","999");
    record_->setValue("HARD_RECORD_DATE","--");
    record_->setValue("HARD_RECORD_TIME","999");
    record_->endGroup();
}

void RecordList::Record(GameMode game_mode,int record_time){
    switch(game_mode){
    case EASY:
        if(record_time < record_label_[2].text().toInt()){
            record_->beginGroup("RECORDS");
            record_->setValue("EASY_RECORD_DATE", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
            record_->setValue("EASY_RECORD_TIME",QString::number(record_time,10));
            record_->endGroup();
        }
        break;
    case NORMAL:
        if(record_time < record_label_[5].text().toInt()){
            record_->beginGroup("RECORDS");
            record_->setValue("NORMAL_RECORD_DATE", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
            record_->setValue("NORMAL_RECORD_TIME",QString::number(record_time,10));
            record_->endGroup();
        }
        break;
    case HARD:
        if(record_time < record_label_[8].text().toInt()){
            record_->beginGroup("RECORDS");
            record_->setValue("HARD_RECORD_DATE", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
            record_->setValue("HARD_RECORD_TIME",QString::number(record_time,10));
            record_->endGroup();
        }
        break;
    default:;
    }
}

void RecordList::Clear(){
    this->Reset();
    this->Read();
}
