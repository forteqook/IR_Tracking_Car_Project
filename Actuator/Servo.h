#ifndef MBED_SERVO_H
#define MBED_SERVO_H

#include "mbed.h"
#define MAX 30.0
// max_left (0) = 0.00113
// max_right (90) : 0.00182
#define MID 0.00148

class Servo
{
private:
    DigitalOut m_PWM;
    Ticker m_period_ticker;
    Timeout m_width_timeout;
    float m_degree;
    float m_width;
    
    void setPeriod();
    void setWidth();
public:
    Servo(PinName IN);
    void update(float degree);
    float getDegree();
};

#endif