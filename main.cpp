#include "mbed.h"
#include "HallSensor.h"
#include "DistanceSensor.h"
#include "Plotting.h"
#include "Motor.h"
#include "Servo.h"
#include "LaneSensor.h"


DistanceSensor pir(p20);
Servo servo (p10);
HallSensor hall (p28);
Motor motor (p23, p24, p25, p26);
LaneSensor laneSensor(p11,p12,p13,p14,p15,p16,p17);
Serial pc (USBTX, USBRX); // tx, rx
Plotting plot;

//Interrupt is generated every 1ms and degree is increased by 1
unsigned int uiFlag_1ms = 0;
unsigned int uiFlag_10ms = 0;

float prev_degree = 0.0;
    
void counter_1ms ()
{
    uiFlag_1ms++;
    uiFlag_10ms++;
}


// Plot distance sensor
int main()
{
    wait(1);

    //Set the 1ms ticker.
    Ticker ticker_1ms;
    ticker_1ms.attach(&counter_1ms,0.001);

    Timer time;
    time.start();
    
    float dis = 0.0;
    float goal_speed = 10.0;
    
    while(1) {
        // Every 1 ms,
        if(uiFlag_1ms >= 1) {
            uiFlag_1ms = 0;
            
            //Set the motor to 10%.
            motor.setSpeed_percent(goal_speed, FORWARD);
            
            dis = pir.getDistance_cm();
            if(dis>6 && dis<30) goal_speed = 0.0;
        }
                
        // Every 10 ms,
        if(uiFlag_10ms>=10) {
            // getError() returns degree
            // lll ll l
            // [3.0] 68.58 [2.0] 45.72 [0.7] 16.002 [0.0] 0.0 -> 30 scale -> 22, 15, 5, 0
            // [3.0, 2.0] 57.15 [2.0, 0.7] 30.861 [0.7, 0.0] 8.001 -> 30 scale -> 19, 10, 2
            // [3.0, 2.0, 0.7] 43.434 [2.0, 0.7, 0.0] 20.574 [0.7, 0.0, -0.7] 0.0 -> 30 scale -> 14, 6, 0
            float error = laneSensor.getError();
            float degree = 0.0;
            error = error / 3.0;
            if(error >= 19.0) {
                degree = 22.0;
            }
            else if(error >= 14.0 && error < 19.0) {
                degree = 15.0;
            }
            else if(error > 5.0 && error < 14.0) {
                degree = 5.0;
            }
            else if(error >= -5.0 && error <= 5.0) {
                degree = 0.0;
            }
            else if(error < -5.0 && error > -14.0) {
                degree = -5.0;
            }
            else if(error <= -14.0 && error > -19.0) {
                degree = -15.0;
            }
            else if(error <= -19.0) {
                degree = -22.0;
            }
            else {
                degree = prev_degree;
            }
            prev_degree = degree;
            servo.update(degree);
        }
    }
}