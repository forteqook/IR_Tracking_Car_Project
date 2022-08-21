#include "LaneSensor.h"

LaneSensor::LaneSensor(PinName lll,PinName ll,PinName l,PinName m,PinName r,PinName rr,PinName rrr)
    : sensorsIn_(lll,ll,l,m,r,rr,rrr)
{
    error_=0.0;
    prevError_=0.0;
    //sensorsIn_.mode(PullNode);
}
float LaneSensor::getError()
{
    bool lll=false;
    bool ll=false;
    bool l=false;
    bool m=false;
    bool r=false;
    bool rr=false;
    bool rrr=false;

    float errorSum=0;
    int errorCount=0;
    float errorConstant =22.86;

    int sensorIn=sensorsIn_ & sensorsIn_.mask();
    if((sensorIn&0x1)==0x0) {
        lll=true;
        errorSum+=errorConstant*3.0;
        errorCount++;
    }
    if((sensorIn&0x2)==0x0) {
        ll=true;
        errorSum+=errorConstant*2.0;
        errorCount++;
    }
    if((sensorIn&0x4)==0x0) {
        l=true;
        errorSum+=errorConstant*0.7;
        errorCount++;
    }
    if((sensorIn&0x8)==0x0) {
        m=true;
        errorSum+=0.0;
        errorCount++;
    }
    if((sensorIn&0x10)==0x0) {
        r=true;
        errorSum+=errorConstant*-0.7;
        errorCount++;
    }
    if((sensorIn&0x20)==0x0) {
        rr=true;
        errorSum+=errorConstant*-2.0;
        errorCount++;
    }
    if((sensorIn&0x40)==0x0) {
        rrr=true;
        errorSum+=errorConstant*-3.0;
        errorCount++;
    }


    if(errorCount==0) {
        error_=prevError_;
    } else {
        error_=errorSum/((float)errorCount);
    }
    prevError_=error_;

    return error_;
}

// BusIn : The order of pins in the constructor is the reverse order of the pins in the byte order.
// If IR sensor detects black line, it returns NC state (not connected).
int LaneSensor::getData()
{
    return sensorsIn_ & sensorsIn_.mask();
}