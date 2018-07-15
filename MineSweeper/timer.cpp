#include "timer.h"

Timer::Timer()
{
    time_number_ = 0;
}

void Timer::TimingStart(){
    timer_1s_.connect(&timer_1s_, SIGNAL(timeout()), this, SLOT(TimingDisplay()));
    timer_1s_.start(1000);
}

void Timer::TimingStop(){
    this->display(time_number_);
    timer_1s_.stop();
    timer_1s_.disconnect(&timer_1s_, SIGNAL(timeout()), this, SLOT(TimingDisplay()));
}

void Timer::TimingDisplay(){
    TimingUpdate();
    this->display(time_number_);
}

void Timer::TimingReset(){
    TimingStop();
    time_number_ = 0;
    this->display(0);
}

void Timer::TimingUpdate(){
    if(time_number_<999)
        time_number_++;
}
