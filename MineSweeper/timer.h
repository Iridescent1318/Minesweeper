#ifndef TIMER_H
#define TIMER_H

#include <QWidget>
#include <QLCDNumber>
#include <QTimer>

class Timer: public QLCDNumber
{
    Q_OBJECT
public:
    Timer();

    void TimingReset();
    void TimingUpdate();
    int GetTiming() const {return time_number_;}
public slots:
    void TimingStart();
    void TimingDisplay();
    void TimingStop();
private:
    QTimer timer_1s_;
    int time_number_;
};



#endif // TIMER_H
